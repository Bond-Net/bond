<div id="1">
  
# [BOND](https://github.com/keybraker/BOND)
![alt text](../img/bond_logo.svg)
<br>

---

1. [BOND](#1)
2. [About BOND](#1)
   1. [Why BOND](#1-1)
   2. [How BOND works](#1-2)
   3. [What does BOND offer](#1-3)
3. [Prerequisites, Build and Clean](#2)
   1. [Prerequisites](#2-1)
   2. [Build](#2-2)
   3. [Clean](#2-3)
4. [Usage Guide](#4)
   1. [Run](#4-1)
   1. [Commands](#4-2)
   2. [Flags](#4-3)
5. [Acknowledgements](#5)

<div id="1">

## About BOND

<div id="2-1">

### Why BOND
As our digital footprints increase by the day, safety is becoming more and more difficult to obtain. Our frequent dilemma is between the safety of a strong password and the ease of remembering our trusted password.<br>
[**BOND**](https://github.com/keybraker/BOND) eliminates this dilemma, not by adding a 
third party as a layer of trust, but by removing it.

* Tro gain the trust of someone is difficult -especially a stranger- 
**good thing you don't need to with [**BOND**](https://github.com/keybraker/BOND).**
* [**BOND**](https://github.com/keybraker/BOND) can function as a per local-device or a 
decentralized-online password manager - the choice is yours.<br>

<div id="2-2">

### How BOND works
[**BOND**](https://github.com/keybraker/BOND) can be used as a local password manager that safely encrypts all your passwords on your computer with one master key you-and-only-you have. Encryptions are compared so that the master key is never stored anywhere.

Passwords are stored on a linked list and their encryption is written to a binary file. Passwords never leave the code without being encrypted to ensure no passwords are ever exposed to the outside world.

Apart from using [**BOND**](https://github.com/keybraker/BOND) as a safe way to store passwords locally, you can always connect to the [**BONDnet**](https://github.com/keybraker/BONDnet). The [**BONDnet**](https://github.com/keybraker/BONDnet) is a program that connects you to the BOND network where segments of password lists is shared across multiple computers.

When connecting to the [**BONDnet**](https://github.com/keybraker/BONDnet) a bond-list with SHA-256 encryption is created with the same master-key. Then fragments the key-vault and distributes them to your bonds in your bond-list.
> more information [**here**](https://github.com/keybraker/BONDnet)

***

This decentralized approach allows for security-of-scale, which means the more bonds 
you have the safer your data is.<br>
Users can store their data online without any-one entity having control over it. 
Bonds are created per-user which allows users to create 
relationships with other [**BONDnet**](https://github.com/keybraker/BONDnet) users.

<div id="2-3">

### What does BOND offer
1. Strong password generator
2. Password storage for as many passwords as you want
3. Password manager

<div id="3">

## Prerequisites, Build and Clean

<div id="3-1">
  
### Prerequisites
1. Make sure you have downloaded `g++` (on macOS `clang++` which is installed alongside Xcode).
2. Download and install openssl (used for encryption and decryption)

<div id="3-2">
  
### Build

1. Clone and move to BOND folder
```bash
git clone https://github.com/keybraker/BOND.git && cd BOND
```

2. Compile BOND
```bash
make
```

<div id="3-3">
  
### Clean

1. In the BOND folder type:
```bash
make clean
```

<div id="4">

## Usage Guide

<div id="4-1">

### Run
The first time you run BOND you will have no key list so you have to make one by pressing ```./bond```. You will be asked to give a master password (which you have to never forget as there is no way to recover your passwords if lost) and then you will can use BOND. All BOND commands can be seen [here](#3-2).

<div id="4-2">

### Commands

| Name            | Argument | Description                                                    |
| :-------------- | :------: | :------------------------------------------------------------- |
| exit/quit/q     |   none   | _terminates execution_                                         |
| reset/rst       |   none   | _will reset your password list by deleting all your passwords_ |
| delete-file     |   none   | _deletes pasword list_                                         |
| insert/i        |   none   | _inserts new password_                                         |
| delete-pass/dlt |   none   | _deletes password_                                             |
| list-all/ls     |   none   | _lists all passwords_                                          |
| list-find/lsf   |   none   | _list password with matching identity_                         |
| edit            |   none   | _lets you update the information of a certain password_        |

<div id="4-3">

### Flags

TBI

<div id="5">

## Acknowledgements

> Author - **Ioannis Tsiakkas** - _[Keybraker](https://github.com/keybraker)_<br>
> License - _[GNU LICENSE](http://www.gnu.org/philosophy/free-sw.html)_<br>

Copyrights © Keybraker 2020 [BOND](https://github.com/keybraker/BOND), All rights reserved

