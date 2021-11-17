import os
import time
import re

if __name__ == '__main__':
    existing_log_files = {}
    while True:
        time.sleep(1)
        for pod_folder in os.listdir("/var/log/pods"):
            for container_folder in os.listdir(os.path.join("/var/log/pods", pod_folder)):
                container_full_path = os.path.join("/var/log/pods", pod_folder , container_folder)
                rotated_file_names = [i for i in os.listdir(container_full_path) if ".gz" not in i and i != "0.log"]
                if container_full_path not in existing_log_files:
                    existing_log_files[container_full_path] = []

                for rotated_file_name in rotated_file_names:
                    if rotated_file_name not in existing_log_files[container_full_path]:
                        existing_log_files[container_full_path].append(rotated_file_name)
                        # print all sequence numbers found
                        filesize_bytes = os.path.getsize(os.path.join(container_full_path, rotated_file_name))
                        with open("/dev/write-to-traces-2", "a") as output_log_file:
                            output_log_file.write("file rotated at " + filesize_bytes + " bytes: " + container_full_path + "__" + rotated_file_name + "\n")
                    
                    # garbage cleanup
                    if len(existing_log_files[container_full_path]) > 5:
                        with open("/dev/write-to-traces-2", "a") as output_log_file:
                            output_log_file.write("garbage collecting " + existing_log_files[container_full_path].pop(0) + "\n")
