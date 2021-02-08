FROM python:3.6

RUN pip install --trusted-host pypi.org --trusted-host files.pythonhosted.org pytest pytest-xdist filelock kubernetes adal msrestazure azure-mgmt-hybridkubernetes azure-mgmt-kubernetesconfiguration

RUN curl https://raw.githubusercontent.com/helm/helm/master/scripts/get-helm-3 | bash \
    && helm version

COPY ./core/arc_conformance.sh /
COPY ./core/pytest.ini /conformancetests/
COPY ./core/conftest.py /conformancetests/
COPY ./core/helper.py /conformancetests/

COPY ./core/test_resource_status.py /conformancetests/

COPY ./common/ /conformancetests/

RUN ["chmod", "+x", "/arc_conformance.sh"]
ENTRYPOINT ["./arc_conformance.sh"]

