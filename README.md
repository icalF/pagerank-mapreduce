# Simple MapReduce Implementation for Calculating PageRank in C++
Coursework for IF4130 Distributed Application Development.
Built using Hadoop Pipe API with C++ workers.

----
## Basic Idea
Source: [Jimmy Lin and Chris Dyer. *Data-Intensive Text Processing with
MapReduce*. Morgan & Claypool Publishers, 2010](
  lintool.github.io/MapReduceAlgorithms)

We would like to calculating rank for Twitter user using follower popularity.
Each user give contribution by his rank to following user.
To calculating rank convergently, we need to iterate the calculating.

We group the followings of each user and update rank iteratively.
Each user propagate rank per number of followings to all following users.
Then, while collecting all contribution from followers, we can update the rank
using [PageRank algorithm](https://en.wikipedia.org/wiki/PageRank).

## Data Format
Data provided by pair of user following another user.
Followers are guaranteed to be single (not duplicating).
Pairs given in format

**user \t follower**

## Usage
### Requirement
* Hadoop environment set ([here](https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/SingleCluster.html#Installing_Software)
is the installation and
[cluster setup guide](https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-common/ClusterSetup.html))
* libssl-dev
* C++ compiler (GCC / LLVM / MSVC)

### Usage
* deploy to Hadoop cluster using ```make deploy```
* setup data on HDFS and change input and output directory
  (including transitition output) inside ```start.sh``` script
* compile using ```make compile```
* run using ```make run```
