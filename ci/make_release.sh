#!/usr/bin/env sh

if [ "x$CG_TOOLCHAIN" = "xclang" ]; then
  echo "clang is not used to upload releases ATM"
  exit 0
fi

curl \
  -H "Accept: application/vnd.github+json" \
  -H "Authorization: Bearer ${GITHUB_TOKEN}" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  "https://api.github.com/repos/bodand/cg3/releases" -o releases.json
draft_id=$(jq '.[] | select(.draft).id' releases.json)

if [ -z "$draft_id" ]; then
  rm -f releases.json
  echo "Draft release not found; creating..."
  jq --null-input \
    --arg tag_name "${CIRRUS_TAG}" \
    --arg name "cg3 release ${CIRRUS_TAG}" \
    --arg body "$(cat ${CIRRUS_WORKING_DIR}/changelog/ChangeLog-${CIRRUS_TAG}.md)" \
    '{"tag_name": $tag_name, "name": $name, "body": $body, "generate_release_notes": false, "draft": true}' \
    >release-payload.json

  curl -X POST \
    -H "Accept: application/vnd.github+json" \
    -H "Authorization: Bearer ${GITHUB_TOKEN}" \
    -H "X-GitHub-Api-Version: 2022-11-28" \
    "https://api.github.com/repos/bodand/cg3/releases" \
    -d "@release-payload.json"

  sleep 5 # idk what is happening anymore...
  curl \
    -H "Accept: application/vnd.github+json" \
    -H "Authorization: Bearer ${GITHUB_TOKEN}" \
    -H "X-GitHub-Api-Version: 2022-11-28" \
    "https://api.github.com/repos/bodand/cg3/releases" -o releases.json
else
  echo "Draft release found; using existing release"
fi

draft_id=$(jq '.[] | select(.draft).id' releases.json)
echo "Draft id used: $draft_id"
echo "GH_REL_ID=$draft_id" >${CIRRUS_ENV}
