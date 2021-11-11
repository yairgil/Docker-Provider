import os
import time
import re

# set up the regex to strip the containerd log headder
header_regex = re.compile("^(.+) (stdout|stderr) (F|P) ", re.MULTILINE)
seq_number_regex = re.compile("Sequence number=([0-9]+)")


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
                        with open("/opt/write-to-traces", "a") as output_log_file:
                            output_log_file.write("file rotated: " + container_full_path + "__" + rotated_file_name)
                    
                    # garbage cleanup
                    if len(existing_log_files[container_full_path]) > 5:
                        with open("/opt/write-to-traces", "a") as output_log_file:
                            output_log_file.write("garbage collecting " + existing_log_files[container_full_path].pop(0))
