#!/usr/bin/env python

import commands
import os
import shlex
import subprocess
import sys
import time

def run(cmdline):
    rc = commands.getstatusoutput(cmdline)
    if rc[0]:
        raise Exception("%s failed: %s: %s" % (cmdline, rc[0], rc[1]))
    return rc[1].strip().split("\n")


def run(command, result=False, debug=False, env=os.environ):            
    if debug:                                                                   
        print(shlex.split(command))
    process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE,    
                                env=env)                                        

    ret = []                                                                    
    while True:                                                                 
        output = process.stdout.readline()                                      
        if output == '' and process.poll() is not None:                         
            break                                                               

        if output:                                                              
            output = output.strip()                                             
            if result:                                                          
                ret.append(output)                                              
            else:                                                               
                print >>sys.stderr, output                                      

    rc = process.poll()                                                         
    if rc:
        raise Exception("%s failed: %s: %s" % (command, rc, "\n".join(ret)))
    if debug:
    	print("%s:%s" % (command, "\n".join(ret)))
    return ret

for pod in run("kubectl get pods --no-headers -o custom-columns=:metadata.name", result=True):
    if "otel" in pod:
        continue

    for line in run("kubectl exec --stdin --tty %s -- /usr/bin/curl http://localhost:5001" % (pod), result=True, debug=False):
#start_time = time.time()
#for line in run("kubectl exec collector-python-564954d845-nkggp -- /bin/bash -c 'for i in {1..20000}; do curl http://localhost:5001; done'", result=True, debug=False):
        print("pod:%s %s" % (pod, line))
#print("--- took %s seconds ---" % (time.time() - start_time))

