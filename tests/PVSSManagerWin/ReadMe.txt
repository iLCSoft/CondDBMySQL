Readme file for installation and usage of the PVSS CondDB Manager

Index

1. Installing the PVSS Manager
   1. Windows
   2. Linux

2. Using the Manager
   1. Creating the configuration datapoint
   2. Configuring the Manager
      1. Database
      2. Datapoint names
      3. The parent folder


1. Installing the PVSS Manager

1.1 Windows

- Unzip the file PVSS00... that matches your version of PVSS
- Place the executable file in the bin directory of your PVSS project
- Make sure you have libmySQL.dll either in the bin directory of PVSS or in
  the bin directory of the project
- Add the Manager in the PVSS console
- Copy the panel file CDBManager_config.pnl to the panel directory of the project

Attention: Only start the manager AFTER you set up the configuration !!!


1.2 Linux

There is currently no manager available for Linux in binary format.

2. Using the manager

2.1 Creating the configuration datapoint

- Run the panel CDBManager_config.pnl
- Click on "Create datapoint"

2.2 Configuring the Manager

There are three things you have to configure, the database, the datapoint
names of the datapoints you want to store and the name of the parent folder.

2.2.1 Database

Use the panel to define the hostname of the MySQL server, the database name,
the user name and the corresponding password.

2.2.2 Datapoint names

To define the datapoint names of the datpoints to be stored there is no tool
available yet. What has to be done is setting the value of
condDBManager.dpoints:_online.._value to a dynamic string which contains the
datapoint names.

2.2.3 The parent folder

You can define a parent folder to separate different systems containing the
same datapoint names. To do this use the panel.

Notes:

The predefined configuration datapoint is condDBManager. This can be changed
by creating a section in the config file that looks like this:

[cdb]

configDP = "condDBManager"

