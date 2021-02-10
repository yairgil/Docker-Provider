import pytest
import os
import time
import pickle

import constants

from filelock import FileLock
from pathlib import Path
# from kubernetes import client, config
from results_utility import create_results_dir, append_result_output
# from arm_rest_utility import fetch_aad_token, fetch_aad_token_credentials
# from helm_utility import pull_helm_chart, export_helm_chart, add_helm_repo, install_helm_chart, delete_helm_release, list_helm_release

pytestmark = pytest.mark.arcagentstest

# Fixture to collect all the environment variables, install pre-requisites. It will be run before the tests.
@pytest.fixture(scope='session', autouse=True)
def env_dict():
    my_file = Path("env.pkl")  # File to store the environment variables.
    with FileLock(str(my_file) + ".lock"):  # Locking the file since each test will be run in parallel as separate subprocesses and may try to access the file simultaneously.
        env_dict = {}
        if not my_file.is_file():
            # Creating the results directory
            create_results_dir('/tmp/results')

            # Setting some environment variables
            env_dict['SETUP_LOG_FILE'] = '/tmp/results/setup'            
            env_dict['TEST_AGENT_LOG_FILE'] = '/tmp/results/containerinsights'
            env_dict['NUM_TESTS_COMPLETED'] = 0

          
            print("Starting setup...")
            append_result_output("Starting setup...\n", env_dict['SETUP_LOG_FILE'])
           
            print("Setup Complete.")
            append_result_output("Setup Complete.\n", env_dict['SETUP_LOG_FILE'])

            with Path.open(my_file, "wb") as f:
                pickle.dump(env_dict, f, pickle.HIGHEST_PROTOCOL)
        else:
            with Path.open(my_file, "rb") as f:
                env_dict = pickle.load(f)
        
    yield env_dict
    
    my_file = Path("env.pkl")
    with FileLock(str(my_file) + ".lock"):
        with Path.open(my_file, "rb") as f:
            env_dict = pickle.load(f)

        env_dict['NUM_TESTS_COMPLETED'] = 1 + env_dict.get('NUM_TESTS_COMPLETED')
        if env_dict['NUM_TESTS_COMPLETED'] == int(os.getenv('NUM_TESTS')):           
            # Checking if cleanup is required.
            if os.getenv('SKIP_CLEANUP'):
                return
            print('Starting cleanup...')
            append_result_output("Starting Cleanup...\n", env_dict['SETUP_LOG_FILE'])
           
            print("Cleanup Complete.")
            append_result_output("Cleanup Complete.\n", env_dict['SETUP_LOG_FILE'])
            return

        with Path.open(my_file, "wb") as f:
            pickle.dump(env_dict, f, pickle.HIGHEST_PROTOCOL)
