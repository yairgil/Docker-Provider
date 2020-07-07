# Release Instructions

Here are the high-level instructions to get the CIPROD`<MM><DD><YYYY>` image for the production release

1. create release branch from ci_dev and make the following updates
      > Note: This required since Azure Dev Ops pipeline doesnt support --build-arg yet to automate this.
   -  Ensure IMAGE_TAG updated with release candiate image tag in the DockerFile under kubernetes/linux and kubernetes/windows directory
   - Update omsagent.yaml if there are any changes to the yaml
   - Release notes
2. Make PR to ci_dev branch and once the PR approved, merge the changes to ci_dev
3. Latest bits of ci_dev automatically deployed to CIDEV cluster in build subscription so just validated E2E to make sure everthing works
4. Merge ci_dev and ci_prod branch which will trigger automatic deployment to CIPROD cluster with CIPROD`<MM><DD><YYYY>` image (TBD)




