require_relative '../test_helpers'
Dir[File.join(File.expand_path(File.dirname(__FILE__)), "../../../../source/code/plugin/health/*.rb")].each { |file| require file }
include HealthModel
include Minitest

describe "HealthHierarchyBuilder spec" do
    it 'builds right hierarchy given a child monitor and a parent monitor provider' do

    end

end