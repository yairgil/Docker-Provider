require 'rake/testtask'

task default: "test"

Rake::TestTask.new do |task|
 task.libs << "test"
 task.pattern = './test/code/plugin/health/*_test.rb'
 task.verbose  = true
end