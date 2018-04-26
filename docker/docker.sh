echo ========= Build docker image
docker build -t otus.lessons.32.01 .
echo ========= Execute kkmeans
docker run --rm -i otus.lessons.32.01 test.sh
echo ========= Remove docker image
docker rmi otus.lessons.32.01