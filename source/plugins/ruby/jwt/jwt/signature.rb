# frozen_string_literal: true

require_relative "security_utils"
require "openssl"
require_relative "algos/hmac"
require_relative "algos/eddsa"
require_relative "algos/ecdsa"
require_relative "algos/rsa"
require_relative "algos/ps"
require_relative "algos/unsupported"
begin
  require "rbnacl"
rescue LoadError
  raise if defined?(RbNaCl)
end

# JWT::Signature module
module JWT
  # Signature logic for JWT
  module Signature
    extend self
    ALGOS = [
      Algos::Hmac,
      Algos::Ecdsa,
      Algos::Rsa,
      Algos::Eddsa,
      Algos::Ps,
      Algos::Unsupported,
    ].freeze
    ToSign = Struct.new(:algorithm, :msg, :key)
    ToVerify = Struct.new(:algorithm, :public_key, :signing_input, :signature)

    def sign(algorithm, msg, key)
      algo = ALGOS.find do |alg|
        alg.const_get(:SUPPORTED).include? algorithm
      end
      algo.sign ToSign.new(algorithm, msg, key)
    end

    def verify(algorithm, key, signing_input, signature)
      algo = ALGOS.find do |alg|
        alg.const_get(:SUPPORTED).include? algorithm
      end
      verified = algo.verify(ToVerify.new(algorithm, key, signing_input, signature))
      raise(JWT::VerificationError, "Signature verification raised") unless verified
    rescue OpenSSL::PKey::PKeyError
      raise JWT::VerificationError, "Signature verification raised"
    ensure
      OpenSSL.errors.clear
    end
  end
end
