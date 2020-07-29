# frozen_string_literal: true

require "net/http"
require "net/https"
require "uri"
require "yajl/json_gem"
require "base64"

class ArcK8sClusterIdentity
  def initialize
    @@crd_resource_uri_template = "%{kube_api_server_url}/apis/clusterconfig.azure.com/v1beta1/namespaces/azure-arc/azureclusteridentityrequests/container-insights-clusteridentityrequest"
    @@secret_resource_uri_template = "%{kube_api_server_url}/api/v1/namespaces/azure-arc/secrets/%{token_secret_name}"
    @token_expiry_time = Time.now
    @cached_access_token = String.new
    @token_secret_name = String.new
    @token_secret_data_name = String.new
    @token_file_path = "/var/run/secrets/kubernetes.io/serviceaccount/token"
    @cert_file_path = "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt"
    @http_client = get_http_client
  end

  def get_cluster_identity_token
    # get the cluster identity token
    if @cached_access_token.to_s.empty? || (Time.now + 60 * 60 > @token_expiry_time) # Refresh token 1 hr from expiration
      kube_api_server_url = get_kube_api_server_url
      crd_request_uri = @@crd_resource_uri_template % {
        kube_api_server_url: kube_api_server_url,
      }
      @service_account_token = get_service_account_token
      get_request = Net::HTTP::Get.new(crd_request_uri)
      get_request["Authorization"] = "Bearer #{@service_account_token}"
      $log.info "Making GET request to #{crd_request_uri} @ #{Time.now.utc.iso8601}"
      get_response = @http_client.request(get_request)
      $log.info "Got response of #{get_response.code} for #{crd_request_uri} @ #{Time.now.utc.iso8601}"

      if get_response.code.to_i == 200
        status = JSON.parse(get_response.body)["status"]
        tokenReference = status["tokenReference"]
        @token_expiry_time = status["expirationTime"]
        @token_secret_name = status["tokenReference"]["secretName"]
        @token_secret_data_name = status["tokenReference"]["dataName"]
      end

      if !@token_secret_name.empty? && !@token_secret_name.empty?
        # get the token from secret
        secret_request_uri = @@secret_resource_uri_template % {
          kube_api_server_url: kube_api_server_url,
          token_secret_name: @token_secret_name,
        }

        get_request = Net::HTTP::Get.new(secret_request_uri)
        get_request["Authorization"] = "Bearer #{@service_account_token}"
        $log.info "Making GET request to #{secret_request_uri} @ #{Time.now.utc.iso8601}"
        get_response = @http_client.request(get_request)
        $log.info "Got response of #{get_response.code} for #{secret_request_uri} @ #{Time.now.utc.iso8601}"
        if get_response.code.to_i == 200
          token_secret = JSON.parse(get_response.body)["data"]
          cluster_identity_token = token_secret[@token_secret_data_name]
          @cached_access_token = Base64.decode64(cluster_identity_token)
        end
      else
        $log.warn ("Failed to get the cluster identity token secret")
      end
    end
    return @cached_access_token
  end

  private

  def get_service_account_token()
    begin
      if File.exist?(@token_file_path) && File.readable?(@token_file_path)
        token_str = File.read(@token_file_path).strip
        return token_str
      else
        $log.info ("Unable to read token string from #{@token_file_path}")
        return nil
      end
    end
  end

  def get_http_client()
    kube_api_server_url = get_kube_api_server_url
    host = URI.parse(kube_api_server_url)
    port = URI.parse(kube_api_server_url)
    http = Net::HTTP.new(host, port)
    http.use_ssl = true
    if !File.exist?(@cert_file_path)
      raise "#{@cert_file_path} doesnt exist"
    else
      http.ca_file = @cert_file_path
    end
    http.verify_mode = OpenSSL::SSL::VERIFY_PEER
    return http
  end

  def get_kube_api_server_url
    if ENV["KUBERNETES_SERVICE_HOST"] && ENV["KUBERNETES_PORT_443_TCP_PORT"]
      return "https://#{ENV["KUBERNETES_SERVICE_HOST"]}:#{ENV["KUBERNETES_PORT_443_TCP_PORT"]}"
    else
      $log.warn ("Kubernetes environment variable not set KUBERNETES_SERVICE_HOST: #{ENV["KUBERNETES_SERVICE_HOST"]} KUBERNETES_PORT_443_TCP_PORT: #{ENV["KUBERNETES_PORT_443_TCP_PORT"]}. Unable to form resourceUri")
      return nil
    end
  end
end
