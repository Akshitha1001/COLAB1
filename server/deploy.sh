#! /bin/bash

#LOAD FUNCTION SCRIPT
source functions.sh

load_env_variables

auth_gcloud

clone_repo_cs

upload_env_file

read_input  # 1 to deploy VM . 2 to delete VM 

CHOICE=$?

if [ ${CHOICE} -eq 1 ]
then

  exe_deploy_cs

elif [ ${CHOICE} -eq 2 ]
then

  exe_clean_cs

fi