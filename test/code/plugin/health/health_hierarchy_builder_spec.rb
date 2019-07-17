require_relative '../test_helpers'
Dir[File.join(__dir__, '../../../../source/code/plugin/health', '*.rb')].each { |file| require_relative file }
include HealthModel
include Minitest

describe "HealthHierarchyBuilder spec" do
    it 'builds right hierarchy given a child monitor and a parent monitor provider' do

    end

end