﻿<p align="center">
  <a href="#"><img src="https://capsule-render.vercel.app/api?type=rect&color=009ACD&height=100&section=header&text=DConsole&fontSize=60%&fontColor=ffffff" alt="website title image"></a>
  <h2 align="center">👉 Dconsole a Debugging Tool👈</h2>
</p>


<p align="center">
<img src="https://img.shields.io/badge/language-Go-blue?style=for-the-badge">
<img src="https://img.shields.io/badge/tech-Websockets-blue?style=for-the-badge">
 </p>
 
 <p align="center">
<img src="https://img.shields.io/github/stars/RAJAGOPALAN-GANGADHARAN/DConsole?style=for-the-badge" >
<img src="https://img.shields.io/github/forks/RAJAGOPALAN-GANGADHARAN/DConsole?style=for-the-badge" >  
<img src="https://img.shields.io/github/issues-raw/RAJAGOPALAN-GANGADHARAN/DConsole?style=for-the-badge" >
<img src="https://img.shields.io/github/issues-pr-closed-raw/RAJAGOPALAN-GANGADHARAN/DConsole?style=for-the-badge" >
</p>

## 📌 Introduction

A Cross platform tool to get multiple logs in one place with easily visualizable colors.
A really useful debugging tool for developers with easy to use GUI scheme.

## ⭐ How to get started?

You can refer to the following articles on the basics of Git and Github and also contact the Project Mentors, in case you are stuck:

- [Watch this video to get started, if you have no clue about open source](https://youtu.be/SL5KKdmvJ1U)
- [Forking a Repo](https://help.github.com/en/github/getting-started-with-github/fork-a-repo)
- [Cloning a Repo](https://help.github.com/en/desktop/contributing-to-projects/creating-a-pull-request)
- [How to create a Pull Request](https://opensource.com/article/19/7/create-pull-request-github)
- [Getting started with Git and GitHub](https://towardsdatascience.com/getting-started-with-git-and-github-6fcd0f2d4ac6)

## 💥 How to Contribute?

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)
[![Open Source Love svg2](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)

- Take a look at the Existing [Issues]
- Wait for the Issue to be assigned to you after which you can start working on it.
- Fork the Repo and create a Branch for any Issue that you are working upon.
- Read the [Code of Conduct]
- Create a Pull Request which will be promptly reviewed and suggestions would be added to improve it.
- Add Screenshots to help us know what this Script is all about.

## Instructions:

#### Requirements

+ Linux

```sh
sudo apt-get install build-essentials qt5-default
```

#### Using Cmake as Generator
```sh
mkdir build && cd build
cmake .. -DCMAKE_CXX_FLAGS="-pthread"
make
```

#### Using Ninja as Generator
```sh
mkdir build && cd build
cmake .. -GNinja -DCMAKE_CXX_FLAGS="-pthread"
ninja -j <CPU_CORES>
```

#### Enable Debug prints
```sh
cmake .. -GNinja -DCMAKE_CXX_FLAGS="-DDEBUG -pthread"
```

#### Communication Channel

For all project related doubts and guidance please join this discord channel

[Discord Link](https://discord.gg/rbpcqxuYzU)


#### Roadmap

As of now simple socket message passing with Bare minimum UI works.

- [ ] Multi language interface

- [ ] Tabbed structure

- [ ] More complex message structure

- [ ] UI(High DPI, Widgets .. etc)

- [ ] Tests

- [ ] Color coded message
