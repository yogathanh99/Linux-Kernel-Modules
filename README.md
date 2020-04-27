# Linux Kernel Modules

## Step 1: Install the Development Environment
On the Ubuntu, we need run the command:

`sudo apt-get install build-essential`

## Step 2: Compile file with `Makefile`. you need run:
`make`

## Step 3: Load new module `randomNumberModule.ko`:
`sudo insmod randomNumberModule.ko`

## Step 4: Run C  code on User Space to retrieve a random number from Kernel Space:
`sudo ./randomNumber_UserSpace`

## Step 5: When you unload the module `randomNumberModule.ko`, you need run:
`sudo rmmod randomNumberModule.ko`