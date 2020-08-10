# frozen_string_literal: true

require_relative "jwt/base64"
require_relative "jwt/json"
require_relative "jwt/decode"
require_relative "jwt/default_options"
require_relative "jwt/encode"
require_relative "jwt/error"
require_relative "jwt/jwk"

# JSON Web Token implementation
#
# Should be up to date with the latest spec:
# https://tools.ietf.org/html/rfc7519
module JWT
  include JWT::DefaultOptions

  module_function

  def encode(payload, key, algorithm = "HS256", header_fields = {})
    Encode.new(payload: payload,
               key: key,
               algorithm: algorithm,
               headers: header_fields).segments
  end

  def decode(jwt, key = nil, verify = true, options = {}, &keyfinder)
    Decode.new(jwt, key, verify, DEFAULT_OPTIONS.merge(options), &keyfinder).decode_segments
  end
end
