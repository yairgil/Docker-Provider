import os
import time
import datetime


def get_CRI_header():
    return datetime.datetime.now().isoformat("T") + "000Z stdout F "

if __name__ == '__main__' and os.environ["DISABLE_PYTHON_LOG_TRACKING"] != "true":
    existing_log_files = {}
    total_bytes_logged = {}
    last_print_time = time.time()
    while True:
        time.sleep(1)
        for pod_folder in os.listdir("/var/log/pods"):
            for container_folder in os.listdir(os.path.join("/var/log/pods", pod_folder)):
                container_full_path = os.path.join("/var/log/pods", pod_folder , container_folder)
                rotated_file_names = [i for i in os.listdir(container_full_path) if ".gz" not in i and i != "0.log"]
                if container_full_path not in existing_log_files:
                    existing_log_files[container_full_path] = []
                    total_bytes_logged[container_full_path] = 0

                for rotated_file_name in rotated_file_names:
                    if rotated_file_name not in existing_log_files[container_full_path]:
                        existing_log_files[container_full_path].append(rotated_file_name)
                        # print all sequence numbers found
                        filesize_bytes = os.path.getsize(os.path.join(container_full_path, rotated_file_name))
                        total_bytes_logged[container_full_path] += filesize_bytes
                        # with open("/dev/write-to-traces", "a") as output_log_file:
                        #     output_log_file.write(get_CRI_header() + "{\"rot_size\": " + str(total_bytes_logged[container_full_path]) + ", \"pod\": \"" + pod_folder + "\", \"container\": \"" + container_folder + "\"}\n")
                    
                    # garbage cleanup
                    if len(existing_log_files[container_full_path]) > 5:
                        existing_log_files[container_full_path].pop(0)
        
        if time.time() - last_print_time > 5:
            last_print_time = time.time()
            with open("/dev/write-to-traces", "a") as output_log_file:
                for container_full_path in existing_log_files.keys():
                    output_log_file.write(get_CRI_header() + "{\"rot_size\": " + str(total_bytes_logged[container_full_path]) + ", \"pod_and_container\": \"" + container_full_path + "\"}\n")

