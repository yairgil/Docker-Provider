python3 ./collector.py &
sleep 30
for x in {1..200000}; do (curl http://localhost:5001) & done
while true; do sleep 30; done;
