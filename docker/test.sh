#!/bin/bash
cat dataset.csv | rclst 100 model100
echo '86.116781;55.335492;2;4326901.00;54.00;7.00;0' | rclss model100