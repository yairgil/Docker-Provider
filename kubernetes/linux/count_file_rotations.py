from prometheus_client import start_http_server, Gauge
import os
import time
import re

# Create a metric to track time spent and requests made.
file_rotation_gauge = Gauge('file_rotation_count', 'number of log files rotated since startup', ["container_name", "pod_name"])
bytes_logged_gauge = Gauge('bytes_rotated', 'number of bytes logged since startup (only counted at lof file rotation)', ["container_name", "pod_name"])

# set up the regex to strip the containerd log headder
header_regex = re.compile("^(.+) (stdout|stderr) (F|P) ", re.MULTILINE)
seq_number_regex = re.compile("Sequence number=([0-9]+)")


if __name__ == '__main__':
    start_http_server(4200)  # Exponse the prometheus metrics
    prev_log_file_names = {}
    gauge_created_containers = set()
    bytes_logged = 0
    while True:
        time.sleep(1)
        for top_folder in os.listdir("/var/log/pods"):
            for sub_folder in os.listdir(os.path.join("/var/log/pods", top_folder)):
                key = os.path.join("/var/log/pods", top_folder , sub_folder)
                log_files = [i for i in os.listdir(key) if ".gz" not in i]
                if len(log_files) == 0:
                    continue  # this would be wird, maybe the container hasn't logged anything yet?
                if len(log_files) == 1:
                    if key not in gauge_created_containers:
                        file_rotation_gauge.labels(container_name=sub_folder, pod_name=top_folder).set(0)
                        gauge_created_containers |= {key}
                        print("created gauge:", sub_folder, top_folder, log_files[0])
                    continue
                
                rotated_file_name = [i for i in log_files if i != "0.log"][0]

                if key in prev_log_file_names.keys():
                    if prev_log_file_names[key] != rotated_file_name:
                        prev_log_file_names[key] = rotated_file_name
                        file_rotation_gauge.labels(container_name=sub_folder, pod_name=top_folder).inc()

                        # add the file size to the total bytes logged count
                        f = open(os.path.join(key, rotated_file_name), "r")
                        data = f.read()  # (reads entire file)
                        data_filtered = re.sub(header_regex, "", data)
                        bytes_logged += len(data_filtered)

                        # bytes_logged += os.stat(os.path.join(key, rotated_file_name)).st_size
                        bytes_logged_gauge.labels(container_name=sub_folder, pod_name=top_folder).set(bytes_logged)

                        # print("updated rotated file:", key)

                        # print all sequence numbers found
                        with open("/opt/write-to-traces", "a") as output_log_file:
                            for seq_num in seq_number_regex.findall(data):
                                output_log_file.write("found sequence number in log file: " + str(seq_num) + "\n")
                else:
                    prev_log_file_names[key] = rotated_file_name
                    # print("added rotated file:", key)
