# Stream Server Analysis (S.A.S.) - Backend

Uses the [OvenMediaEngine](https://github.com/AirenSoft/OvenMediaEngine) to handle incoming live RTSP Feeds and relay them to users which can watch them using [SAS-Client](https://github.com/joaobal/sas-client). 
Project overview: [SAS](https://github.com/joaobal/stream-server-analysis)

## Contents

- [SAS-Server](#stream-server-analysis-sas-backend)
  - [Contents](#contents)
  - [Description](#description)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [How to use](#how-to-use)
  - [Deploy](#deploy)
- [Development](#development)
  - [Tests](#tests)
  - [Branching](#branching)

---

## Description

SAS-Server is a media server which also processes/analyses the stream according to requests sent by the user.

## Install

## How to use

## Deploy

---

## Development

### Tests

### Branching

GitFlow:

- master: This branch always reflects a production-ready state. Only release-ready code is merged here.
- dev: This is the primary development branch where all completed features are merged. It represents the latest delivered development changes for the next release.
- feature/*: These branches are created from dev for working on new features. They are merged back into dev when the feature is complete.
- release/*: When dev has enough features for a release (or a release date is approaching), a release branch is created from dev. This branch is used for final testing, bug fixes, and preparing release metadata. Once ready, it's merged into master (and tagged) and also back into dev (to incorporate any bug fixes made in the release branch).
- hotfix/: These branches are created from master to quickly patch production issues. Once fixed, the hotfix is merged back into both master and dev (or the current release branch).

