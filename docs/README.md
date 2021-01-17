<h1 align="center">Bond</h1>

<p align="center">
  As our digital footprints increase by the day, safety is becoming more and more difficult to obtain.<br>
  Our frequent dilemma is between the safety of a strong password and the ease of remembering our trusted password
</p>

<p align="center">
  Bond eliminates this dilemma, not by adding a third party as a layer of trust, but by removing it.
</p>

<p align="center">
  To gain the trust of someone is difficult, especially a stranger, <b>good thing you don't need to with Bond</b><br>
  Bond can function as a per local-device or a decentralized-online password manager - the choice is yours.<br>
</p>

---

### How Bond works
Bond can be used as a local password manager that safely encrypts all your passwords on your computer with one master key you-and-only-you have. Encryptions are compared so that the master key is never stored anywhere.

Passwords are stored on a linked list and their encryption is written to a binary file. Passwords never leave the code without being encrypted to ensure no passwords are ever exposed to the outside world.

Apart from using Bond as a safe way to store passwords locally, you can always connect to the BondNet. The BondNet is a program that connects you to the BOND network where segments of password lists is shared across multiple computers.

When connecting to the BondNet a bond-list with SHA-256 encryption is created with the same master-key. Then fragments the key-vault and distributes them to your bonds in your bond-list.
> for more information about BondNet click [**here**](https://github.com/keybraker/BONDnet)

---

This decentralized approach allows for security-of-scale, which means the more bonds 
you have the safer your data is.<br>
Users can store their data online without any-one entity having control over it. 
Bonds are created per-user which allows users to create 
relationships with other BondNet users.

### What does Bond offer
1. Strong password generator
2. Password storage for as many passwords as you want
3. Password manager

## Prerequisites, Build and Clean
  
### Prerequisites
1. Make sure you have downloaded `g++` (on macOS `clang++` which is installed alongside Xcode).
2. Download and install openssl (used for encryption and decryption)
  
### Build

1.  Clone and move to Bond folder

        $ git clone https://github.com/keybraker/BOND.git && cd BOND
        
2.  Compile Bond

        $ make
  
### Clean

3. In the Bond folder type

        $ make clean

## Usage Guide

### Execute Bond
The first time you run BOND you will have no key list so you have to make one by pressing ```./bond```. You will be asked to give a master password (which you have to never forget as there is no way to recover your passwords if lost) and then you will can use BOND. All BOND commands can be seen [here](#3-2).

4. Execute

        $ ./bond

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

### Flags

TBI

## Acknowledgements

> Author - **Ioannis Tsiakkas** - _[Keybraker](https://github.com/keybraker)_<br>
> License - _[GNU LICENSE](http://www.gnu.org/philosophy/free-sw.html)_<br>

<p align="center">
  Copyrights © Keybraker 2020-2021 Bond, All rights reserved
</p>
