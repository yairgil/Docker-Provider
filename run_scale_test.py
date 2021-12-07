#!/usr/bin/python3
from os import error
import os
import subprocess
import time
import re
import sys
from itertools import product
import traceback
import random
from colorama import Fore

import pdb

MIN_PYTHON = (3, 0)
if sys.version_info < MIN_PYTHON:
    sys.exit("Python %s.%s or later is required.\n" % MIN_PYTHON)


def run_command_get_output(cmd):
    # print(Fore.GREEN + "\t\t\t[debug] running command: " + cmd + Fore.RESET)
    process = subprocess.Popen(["bash", "-c", cmd], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    output, error = process.communicate()
    if error is not None and len(error) > 0:
        print(error)
        raise ValueError("some error occured")
    return output.decode('utf-8').strip()


def dict_values_sorted_by_keys(d):
    return [str(d[k]) for k in sorted(d.keys())]

def dict_product(d: dict):
    keys_ordered = [i for i in d.keys()]
    # print("keys_ordered: ", keys_ordered)
    lst = [d[k] for k in keys_ordered]
    # print("lst: ", lst)
    for comb in product(*lst):
        yield {keys_ordered[i]: comb[i] for i in range(len(comb))}

def validate_setting_combos(settings):
    if str(settings["AGENT_IMAGE"]).startswith("mcr.microsoft.com"):
        if str(settings["DISABLE_LOG_TRACKING"]) == "false":
            return False
        if str(settings["DISABLE_PYTHON_LOG_TRACKING"]) == "false":
            return False
    return True

def main():
    print("starting AKS cluster (could take a long time)")
    run_command_get_output("az aks start -g davidscaletest_group -n davidscaletest")

    settings = {"LOG_WRITER_REPLICAS": ["40"] * 40, 
                "DISABLE_LOG_TRACKING": ["false", "true"], 
                # "DISABLE_LOG_TRACKING": ["false"], 
                # "DISABLE_PYTHON_LOG_TRACKING": ["false", "true"],
                "DISABLE_PYTHON_LOG_TRACKING": ["true"],
                # "AGENT_IMAGE": ["mcr.microsoft.com/azuremonitor/containerinsights/ciprod:ciprod10132021", "davidmichelman/countrotations:v67"]
                "AGENT_IMAGE": ["davidmichelman/countrotations:v67"]
                # "AGENT_IMAGE": ["mcr.microsoft.com/azuremonitor/containerinsights/ciprod:ciprod10132021"]
                }

    data_items_multiple = {"cpu_usage_fbit_agg",
                    "mem_usage_fbit_agg",
                    "cpu_usage_python_agg",
                    "mem_usage_python_agg",
                    "cpu_usage_mdsd_agg",
                    "mem_usage_mdsd_agg",
                    "cpu_usage_fluentd_worker_agg",
                    "mem_usage_fluentd_worker_agg",
                    "cpu_usage_fluentd_supervisor_agg",
                    "mem_usage_fluentd_supervisor_agg",
                    "cpu_usage_telegraf_agg",
                    "mem_usage_telegraf_agg",
                    "allup_pod_cpu_agg",
                    "allup_pod_mem_agg",
                    "disk_reads_per_sec_agg",
                    "disk_writes_per_sec_agg",
                    "disk_kb_read_per_sec_agg",
                    "disk_kb_write_per_sec_agg"}
    
    data_items_single = {"kusto_bytes_lost_per_minute", "kusto_bytes_sent_per_minute"}

    with open("results_file.csv", "a") as results_file:
        results_file.write(", ".join(sorted(settings.keys())))
        results_file.write(", ")
        results_file.write(", ".join(sorted(data_items_multiple)))
        results_file.write(", ")
        results_file.write(", ".join(sorted(data_items_single)))
        results_file.write("\n")

    all_setting_combos = list(dict_product(settings))
    random.shuffle(all_setting_combos)
    for i, run_settings in enumerate(all_setting_combos):
        if validate_setting_combos(run_settings):
            print("starting run " + str(i) + " with settings: ", run_settings)
            run_test(run_settings)
        else:
           print("skipping run with invalid settings: ", run_settings) 
    
    # shut down the cluster when done to save a bit of money (although not much compared to the LA data this uses)
    run_command_get_output("az aks stop -g davidscaletest_group -n davidscaletest")


def run_test(settings, monitor_time_minutes=10):
    try:
        run_command_get_output("kubectl delete -f scale-test.yaml")

        while len(run_command_get_output("kubectl get pods -n kube-system | grep omsagent")) > 0:
            print("waiting for previous omsagent pods to stop")
            time.sleep(10)
        time.sleep(0.5)


        with open("scale-test.yaml", "r") as kube_file:
            scale_test_definition = kube_file.read()
        for k, v in settings.items():
            scale_test_definition = scale_test_definition.replace("<" + k + ">", v)

        run_command_get_output("rm scale-test-specific.yaml")

        with open("scale-test-specific.yaml", "w") as output_file:
            output_file.write(scale_test_definition)

        run_command_get_output('kubectl apply -f scale-test-specific.yaml')

        # wait for all logger pods to start up. This can take a while when there are a lot
        while int(run_command_get_output("kubectl get pods -n default | grep -v Running | wc -l")) > 1:
            time.sleep(1)
        
        time.sleep(1)

        ds_pod_name = run_command_get_output('kubectl get pods -n kube-system | grep omsagent | grep -v omsagent-rs | xargs | cut -d " " -f1')
        print("ds pod: " + ds_pod_name)

        # ds_pod_name = run_command_get_output('kubectl exec ' + ds_pod_name + " -c omsagent -- apt-get install -y sysstat")

        if len(ds_pod_name) < len("omsagent-xxxxx"):
            print("failed to get omsagent pod name")
            pdb.set_trace()
            pass
            pass

        while len(run_command_get_output('kubectl exec ' + ds_pod_name + ' -c omsagent -- echo "container ready"')) < len("container ready"):
            time.sleep(5)
        
        recorded_results_agg = {}
        recorded_results_single = {}

        recorded_results_agg["cpu_usage_fbit_agg"] = []
        recorded_results_agg["mem_usage_fbit_agg"] = []
        recorded_results_agg["cpu_usage_python_agg"] = []
        recorded_results_agg["mem_usage_python_agg"] = []
        recorded_results_agg["cpu_usage_mdsd_agg"] = []
        recorded_results_agg["mem_usage_mdsd_agg"] = []
        recorded_results_agg["cpu_usage_fluentd_worker_agg"] = []
        recorded_results_agg["mem_usage_fluentd_worker_agg"] = []
        recorded_results_agg["cpu_usage_fluentd_supervisor_agg"] = []
        recorded_results_agg["mem_usage_fluentd_supervisor_agg"] = []
        recorded_results_agg["cpu_usage_telegraf_agg"] = []
        recorded_results_agg["mem_usage_telegraf_agg"] = []
        recorded_results_agg["allup_pod_cpu_agg"] = []
        recorded_results_agg["allup_pod_mem_agg"] = []

        recorded_results_agg["disk_reads_per_sec_agg"] = []
        recorded_results_agg["disk_writes_per_sec_agg"] = []
        recorded_results_agg["disk_kb_read_per_sec_agg"] = []
        recorded_results_agg["disk_kb_write_per_sec_agg"] = []

        last_loop_end_time = time.time()

        start_time = time.time()
        end_time = start_time + monitor_time_minutes * 60

        for i in range(monitor_time_minutes * 2):
            time.sleep(max(last_loop_end_time + 30 - time.time(), 0))
            try:
                get_pid_command = 'kubectl exec ' + ds_pod_name + """ -c omsagent -- ps -o pid,command ax | grep '<proc_name>' | grep -v grep | xargs | cut -d " " -f1"""
                fbit_pid = run_command_get_output(get_pid_command.replace("<proc_name>", "td-agent-bit"))
                python_pid = run_command_get_output(get_pid_command.replace("<proc_name>", "python"))
                mdsd_pid = run_command_get_output(get_pid_command.replace("<proc_name>", "mdsd"))
                fluentd_worker_pid = run_command_get_output(get_pid_command.replace("<proc_name>", "/usr/bin/ruby2.6 -Eascii-8bit:ascii-8bit /usr/local/bin/fluentd"))
                fluentd_supervisor_pid = run_command_get_output(get_pid_command.replace("<proc_name>", "/usr/bin/ruby2.6 /usr/local/bin/fluentd"))
                telegraf_pid = run_command_get_output(get_pid_command.replace("<proc_name>", "telegraf"))

                if len(fbit_pid) < 1:
                    # container hasn't started up yet, skip this iteration
                    print("container hasn't started yet, skipping iteration")
                    continue

                get_cpu_command = 'kubectl exec ' + ds_pod_name + ' -c omsagent -- ps -o %cpu,pid ax | grep <pid> | grep -v grep |  xargs | cut -d " " -f1'
                get_mem_command = 'kubectl exec ' + ds_pod_name + ' -c omsagent -- ps -o rss,pid ax | grep <pid> | grep -v grep |  xargs | cut -d " " -f1'

                try:
                    cpu_usage_fbit = run_command_get_output(get_cpu_command.replace("<pid>", fbit_pid))
                    recorded_results_agg["cpu_usage_fbit_agg"].append(float(cpu_usage_fbit))
                    mem_usage_fbit = run_command_get_output(get_mem_command.replace("<pid>", fbit_pid))
                    recorded_results_agg["mem_usage_fbit_agg"].append(float(mem_usage_fbit))

                    cpu_usage_mdsd = run_command_get_output(get_cpu_command.replace("<pid>", mdsd_pid))
                    recorded_results_agg["cpu_usage_mdsd_agg"].append(float(cpu_usage_mdsd))
                    mem_usage_mdsd = run_command_get_output(get_mem_command.replace("<pid>", mdsd_pid))
                    recorded_results_agg["mem_usage_mdsd_agg"].append(float(mem_usage_mdsd))

                    cpu_usage_fluentd_worker = run_command_get_output(get_cpu_command.replace("<pid>", fluentd_worker_pid))
                    recorded_results_agg["cpu_usage_fluentd_worker_agg"].append(float(cpu_usage_fluentd_worker))
                    mem_usage_fluentd_worker = run_command_get_output(get_mem_command.replace("<pid>", fluentd_worker_pid))
                    recorded_results_agg["mem_usage_fluentd_worker_agg"].append(float(mem_usage_fluentd_worker))

                    cpu_usage_fluentd_supervisor = run_command_get_output(get_cpu_command.replace("<pid>", fluentd_supervisor_pid))
                    recorded_results_agg["cpu_usage_fluentd_supervisor_agg"].append(float(cpu_usage_fluentd_supervisor))
                    mem_usage_fluentd_supervisor = run_command_get_output(get_mem_command.replace("<pid>", fluentd_supervisor_pid))
                    recorded_results_agg["mem_usage_fluentd_supervisor_agg"].append(float(mem_usage_fluentd_supervisor))
                    
                    cpu_usage_telegraf = run_command_get_output(get_cpu_command.replace("<pid>", telegraf_pid))
                    recorded_results_agg["cpu_usage_telegraf_agg"].append(float(cpu_usage_telegraf))
                    mem_usage_telegraf = run_command_get_output(get_mem_command.replace("<pid>", telegraf_pid))
                    recorded_results_agg["mem_usage_telegraf_agg"].append(float(mem_usage_telegraf))
                    
                    print("cpu_usage_fbit: " + cpu_usage_fbit + " (%), mem_usage_fbit: " + mem_usage_fbit + " (kb)")

                    if len(python_pid) > 0:
                        cpu_usage_python = run_command_get_output(get_cpu_command.replace("<pid>", python_pid))
                        recorded_results_agg["cpu_usage_python_agg"].append(float(cpu_usage_python))

                        mem_usage_python = run_command_get_output(get_mem_command.replace("<pid>", python_pid))
                        recorded_results_agg["mem_usage_python_agg"].append(float(mem_usage_python))
                        # print("cpu_usage_python: " + cpu_usage_python + " (%), mem_usage_python: " + mem_usage_python + " (kb)")

                except Exception as e:
                    print("error reading cpu/mem for processes stats: " + str(e))
                    traceback.print_exc()


                try:
                    disk_usage_raw = run_command_get_output('kubectl exec ' + ds_pod_name + ' -c omsagent -- iostat -d sda -x | grep sda')
                    disk_reads_per_sec, disk_writes_per_sec, disk_kb_read_per_sec, disk_kb_write_per_sec = re.findall("sda\s+([0-9.]+)\s+([0-9.]+)\s+([0-9.]+)\s+([0-9.]+)", disk_usage_raw)[0]
                    recorded_results_agg["disk_reads_per_sec_agg"].append(float(disk_reads_per_sec))
                    recorded_results_agg["disk_writes_per_sec_agg"].append(float(disk_writes_per_sec))
                    recorded_results_agg["disk_kb_read_per_sec_agg"].append(float(disk_kb_read_per_sec))
                    recorded_results_agg["disk_kb_write_per_sec_agg"].append(float(disk_kb_write_per_sec))

                    print("disk_reads_per_sec, disk_writes_per_sec, disk_kb_read_per_sec, disk_kb_write_per_sec: ", disk_reads_per_sec, disk_writes_per_sec, disk_kb_read_per_sec, disk_kb_write_per_sec)
                except Exception as e:
                    print("error reading disk stats: " + str(e))
                    traceback.print_exc()

                try:
                    allup_pod_usage = run_command_get_output("kubectl top pod -n kube-system " + ds_pod_name + " | grep -v NAME")
                    allup_pod_cpu, allup_pod_mem = re.findall("\S+\s+(\S+)\s+(\S+)", allup_pod_usage)[0]
                    recorded_results_agg["allup_pod_cpu_agg"].append(float(str(allup_pod_cpu).replace("m", "")))
                    recorded_results_agg["allup_pod_mem_agg"].append(float(str(allup_pod_mem).replace("Mi", "")))

                    print("allup_pod_cpu: " + allup_pod_cpu + ", allup_pod_mem: " + allup_pod_mem)
                except (ValueError, IndexError) as e:
                    print("no kubernetes performance metrics yet")
                    traceback.print_exc()

                print("")
            except error as e:
                print(e)
        
            last_loop_end_time = time.time()

        print("done logging, waiting a minute for logs to show up in kusto")
        # end_time = time.time()
        time.sleep(90)
        
        query_logs_lost = """ContainerLog
                            | where TimeGenerated > datetime(1970-01-01) + <start_time> * 1sec
                            | where TimeGenerated < datetime(1970-01-01) + <end_time> * 1sec
                            | extend sequence_raw = tolong(extract("Sequence number=([0-9]+) random", 1, LogEntry))
                            | where not(isempty(sequence_raw))
                            | extend sequence = tolong(sequence_raw)
                            | sort by ContainerID, sequence asc
                            | extend nextSeq = next(sequence, 1), nextContainerID = next(ContainerID, 1)
                            | extend bytes_lost = (nextSeq - sequence - 1) * strlen(LogEntry)
                            | where ContainerID == nextContainerID and bytes_lost > 1
                            | summarize sum_bytes_lost=sum(bytes_lost) / (toreal(<end_time> - <start_time>) / 60)
                            """.replace("<start_time>", str(int(start_time))).replace("<end_time>", str(int(end_time)))

        bytes_lost_raw = run_command_get_output("az monitor log-analytics query --workspace c3b6a23a-7e35-4884-b307-c1ebf0c1e025 --analytics-query '" + query_logs_lost + "' | grep sum_bytes_lost".replace("\n", ""))
        recorded_results_single["kusto_bytes_lost_per_minute"] = float(re.findall('"sum_bytes_lost": "([0-9.]+)"', bytes_lost_raw)[0])
        print("bytes_lost: " + str(recorded_results_single["kusto_bytes_lost_per_minute"]))

        query_logs_recieved = """ContainerLog
                                | where TimeGenerated > datetime(1970-01-01) + <start_time> * 1sec
                                | where TimeGenerated < datetime(1970-01-01) + <end_time> * 1sec
                                | extend sequence_raw = extract("Sequence number=([0-9]+) random", 1, LogEntry), dummy = 1
                                | where not(isempty(sequence_raw))
                                | summarize byte_count = sum(strlen(LogEntry)) / (toreal(<end_time> - <start_time>) / 60)
                                """.replace("<start_time>", str(int(start_time))).replace("<end_time>", str(int(end_time)))

        bytes_in_kusto_raw = run_command_get_output("az monitor log-analytics query --workspace c3b6a23a-7e35-4884-b307-c1ebf0c1e025 --analytics-query '" + query_logs_recieved + "' | grep byte_count".replace("\n", ""))
        recorded_results_single["kusto_bytes_sent_per_minute"] = float(re.findall('"byte_count": "([0-9.]+)"', bytes_in_kusto_raw)[0])
        print("logs_lost: " + str(recorded_results_single["kusto_bytes_sent_per_minute"]))
        
        print(run_command_get_output("kubectl delete -f scale-test-specific.yaml"))

        recorded_results_agg_avg = {key: float(sum(value)) / (len(value) + 0.00001) for (key, value) in recorded_results_agg.items()}

        # write results to csv file
        with open("results_file.csv", "a") as results_file:
            results_file.write(", ".join(dict_values_sorted_by_keys(settings)))
            results_file.write(", ")
            results_file.write(", ".join(dict_values_sorted_by_keys(recorded_results_agg_avg)))
            results_file.write(", ")
            results_file.write(", ".join(dict_values_sorted_by_keys(recorded_results_single)))
            results_file.write("\n")

        # return {key: float(sum(value)) / len(value) for (key, value) in recorded_results_agg.items()}, recorded_results_single
    except error as e:
        print("some error: " + str(e))
        traceback.print_exc()
        run_command_get_output("kubectl delete -f scale-test.yaml")
        pdb.set_trace()
        pass
    except Exception as e:
        print("some error: " + str(e))
        traceback.print_exc()
        run_command_get_output("kubectl delete -f scale-test.yaml")
        pdb.set_trace()
        pass
    print()


if __name__ == "__main__":
    main()
