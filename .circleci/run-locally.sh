#!/bin/bash -e

curl --user ${TOKEN}: \
    --request POST \
    --form revision=$1\
    --form config=@config.yml \
    --form notify=false \
        https://circleci.com/api/v1.1/project/github/dimtass/DSP-Cpp-filters/tree/$2