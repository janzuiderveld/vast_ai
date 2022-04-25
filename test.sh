kill -9 $(lsof -t -i:8080)

cmd=`cat midialogue/ssh_pipe.cmd` 
$cmd > test_ssh.thrash &

sleep 3

echo $(lsof -i:8080)

cd midialogue
python3 ./Python-TCP-Image-Socket/client.py --dummy 1 2>&1 | tee _client_send.log &