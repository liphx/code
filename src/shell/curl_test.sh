#!/bin/bash

rm -f test.db

make
./main 127.0.0.1 5000 --test > /dev/null 2>&1 &
pid_of_main=$!

sqlite3 test.db <<EOF
DROP TABLE IF EXISTS user;
create table IF NOT EXISTS user(name string, passwd string, email string, PRIMARY KEY (name));
delete from user;

DROP TABLE IF EXISTS friends;
create table IF NOT EXISTS friends(name1 string, name2 string, PRIMARY KEY (name1, name2));
delete from friends;

DROP TABLE IF EXISTS message;
create table IF NOT EXISTS message(name1 string, name2 string, message string, time int);
delete from message;
EOF

function curl_post() {
    url=$1
    data=$2
    curl --header "Content-Type: application/json" --request POST --data "${data}" "${url}" 2> /dev/null
}

url_prefix='http://127.0.0.1:5000'
test_set_api=(
    '/api/register'
    '/api/register'
    '/api/login'
    '/api/islogin'
    '/api/information'
    '/api/alter_password'
    '/api/alter_information'
    '/api/information'
    '/api/add_friend'
    '/api/login'
    '/api/apply_friend'
    '/api/deal_friend'
    '/api/get_friends'
    '/api/get_friends'
    '/api/send_message'
    '/api/send_message'
    '/api/get_message'
    '/api/get_message'
)

test_set_data=(
    '{ "name": "abc", "passwd": "123", "email": "123@abc.com" }'
    '{ "name": "ABC", "passwd": "123", "email": "123@abc.com" }'
    '{ "name": "abc", "passwd": "123" }'
    '{ "name": "abc", "session_id": "123456" }'
    '{ "name": "abc", "session_id": "123456" }'
    '{ "name": "abc", "session_id": "123456", "old_passwd": "123", "new_passwd": "321" }'
    '{ "name": "abc", "session_id": "123456", "email": "123@cba.com" }'
    '{ "name": "abc", "session_id": "123456" }'
    '{ "name": "abc", "session_id": "123456", "friend_name": "ABC" }'
    '{ "name": "ABC", "passwd": "123" }'
    '{ "name": "ABC", "session_id": "123456" }'
    '{ "name": "ABC", "session_id": "123456", "friend_name": "abc", "agree": true }'
    '{ "name": "abc", "session_id": "123456" }'
    '{ "name": "ABC", "session_id": "123456" }'
    '{ "name": "ABC", "session_id": "123456", "friend_name": "abc", "message": "hello" }'
    '{ "name": "abc", "session_id": "123456", "friend_name": "ABC", "message": "world" }'
    '{ "name": "ABC", "session_id": "123456", "friend_name": "abc", "how": 0 }'
    '{ "name": "abc", "session_id": "123456", "friend_name": "ABC", "how": 1 }'

)

test_set_expect=(
    '{"status":true}'
    '{"status":true}'
    '{"session_id":"123456","status":true}'
    '{"status":true}'
    '{"information":{"email":"123@abc.com","name":"abc"}}'
    '{"status":true}'
    '{"status":true}'
    '{"information":{"email":"123@cba.com","name":"abc"}}'
    '{"status":true}'
    '{"session_id":"123456","status":true}'
    '{"request":["abc"]}'
    '{"status":true}'
    '{"friends":["ABC"]}'
    '{"friends":["abc"]}'
    '{"status":true}'
    '{"status":true}'
    '{"message":[{"message":"hello","name1":"ABC","name2":"abc","time":0},{"message":"world","name1":"abc","name2":"ABC","time":0}]}'
    '{"message":[{"message":"hello","name1":"ABC","name2":"abc","time":0}]}'
)

function test_main() {
    let "pass = 0"
    for i in ${!test_set_api[@]}; do
      result=$(curl_post "${url_prefix}${test_set_api[$i]}" "${test_set_data[$i]}")
      if [ "$result" = "${test_set_expect[$i]}" ]; then
          let "pass = pass + 1"
      else
          echo "--------test case $(($i + 1)) failed--------"
          echo "$result"
      fi
    done
    echo "${#test_set_api[@]} cases, $pass passed"
}

test_main

sqlite3 test.db <<EOF
SELECT * FROM user;
SELECT * FROM friends;
SELECT * FROM message;
EOF

kill -9 $pid_of_main
rm -f test.db
exit 0
