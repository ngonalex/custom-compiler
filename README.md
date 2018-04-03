
You've connected to CSIL or fired up your local terminal and want to start writing your own compiler? Good! This is a handout that will help you along the way.

Our goal for now is to make sure that your team, your partner team and the administrative TA team are all in sync and functional. We're setting up a somewhat complex environment and making sure we start off right is very important.

At the end of this handout, you should be ready to take over and do some serious development in an organized way.

Prerequisites
=============

The following software must be installed before we can proceed with actual work:

- git
- g++
- googletest
- bazel
- cppcheck

The steps for installing all of these are easily googlable, so make sure you have that ready before proceeding.

Step 1: Making a project repository
===================================

Go to the [UCSB Github](https://github.ucsb.edu/) website and create a private repository within the **CS160-S18** organization. Make up a team name and call the repository `team-<name>`. Be original. If your Github account isn't added to this organization, please contact any of the TAs to fix that as soon as possible. You **cannot** go on without first finishing this step. 

Your new repository is ready, but quite empty. We've created a good foundation for your team, complete with testing, continuous integration and bazel configurations.

To get this into our private repo, we will do what is known as a mirror-push. To start off, clone our template repository bare:

```git clone --bare https://github.ucsb.edu/CS160-S18/team-tunafish.git```

Next, go into it and mirror push to your new repository, like so:

```
cd team-tunafish.git
git push --mirror https://github.ucsb.edu/CS160-S18/team-<name>.git
cd ..
rm -rf team-tunafish.git
```

If you look at your project in Github, you'll see that it's no longer empty! It now contains all the setup neccesary for you to start hacking away at a compiler!

Go to Settings > Hooks > Webhooks on the Github repo page, and add a Webhook. Configure it like this:

```
Payload URL: http://es.cs.ucsb.edu:16000
Content Type: application/json
Secret: <leave empty>
Which events? Send me **everything**
Active: yes!
```

This will allow you to see whether continuous integration is set up correctly for your repo, and to run automated tests.

Congratulations! The next step would be to clone the new repo and settle in:

```git clone https://github.ucsb.edu/CS160-S18/team-<name>.git```

WORKING ON CSIL? INSTALL BAZEL LIKE THIS!
=========================================

Go to your home directory, and then do the following:

```
mkdir bazel
cd bazel
wget https://github.com/bazelbuild/bazel/releases/download/0.11.1/bazel-0.11.1-dist.zip
./compile.sh
cd ..
echo "alias bazel=~/bazel/output/bazel" >> ~/.bashrc
```

This should install it locally and get you ready to use it from now on!
Do this line by line, so that you can catch any error that happens.

OKAY, GOING FORWARD!
====================

Step 3: Compiling, fixing, recompiling
======================================

You can now try compiling the project. To compile the project, we're using 
**bazel**. If you don't know how to use it, a tutorial is included [here](link). Entering the directory, we can now run:

```bazel build rcc:ucsb-rcc```

to compile the project.

To test locally, you can run the following:

```bazel test rcc:ucsb-rcc-test```

Step 3a: Branch out
-------------------

By default, your work will always end on the master branch of the repository. The remote master branch, in the end, the only one we will care about when evaluating your work.

When fixing things, however, you will always be creating so-called *feature branches*: small branches that should only contain one change at a time. To create a new branch, we do:

```git checkout -b <branch-name>```

Now whatever change you commit, it will be saved in this branch. This also means that, should we have to return to a previous point, we are somewhat safe from massive data loss. Don't be afraid of making the branch names as descriptive as possible. If you are using a nice shell or git integration software, the shell itself will tell you what branch you're on, so that you don't get lost.

Step 3b: Fix the error, commit and push
---------------------------------------

It's time now to fix the error that's stopping the tests from passing. In our case, this one is simple: to figure it out, open the only source file and follow the comments there. Spoiler alert: it's really simple.

Once you're done, recompile the code and if compilation passes, commit by doing:

```git add --all .
git commit -m "<very informative message here>"
```
Once you're happy (this might be after the first commit, or the 10th, or the 100th), push the code to the remote branch by doing:

```git push origin <branch-name>```

After this, if you go back to the Github page, it will show you that your code has been commited and suggest that you "Review & Pull Request". This is the second most important part of the dev cycle: in this step, you will make a request for the master branch to pull your changes and accept them in full. This process is guarded by reviews and tests.

Step 3c: Sending out pull requests for review
---------------------------------------------

If you've never made pull requests, read about them [here](https://help.github.com/articles/creating-a-pull-request/). Basically, you'll choose several people to review your changes to the code, and if everyone agrees that the changes are fine (LGTMs them -- "Looks Good To Me"), the branch is automatically tested, and if that passes, it is merged into master. Congratulations!

Take the review process seriously. Your grade, and the future quality of your compiler, depends heavily on it. When choosing people for the review process, always pick **one** person from your side of the team (frontend or backend), **two** people from the other side of the the team (backend or frontend) and one TA. The review requires a TA for two reasons: so that we can check that your selection of reviewers is valid, and to have a simple statistics of the frequency of pushes, pull requests and reviews -- the activity of your project.

[Here](https://www.youtube.com/watch?v=HW0RPaJqm4g)'s a nice video explaining code reviews. Be nice, but don't be afraid to put your foot down and not let bad practice go through. The automatic tools that we've setup for you will take care of many things, but sometimes, you are the only ones stopping bad practices from happening. You will notice soon enough that the larger the project gets, the harder it is to live with the small hacks you do early on. The review process should stop such activities all-together.

Once you meet all of these requirements, your request will go through, and the branch will be merged with master remotely. Don't forget to return to master locally too, pull and start the next change from there!

At this point, you have learned how to submit your code, and how to work with your team. This method might seem taxing and wasting time, but it is a straight-forward way of keeping a sane head once two teams of three people start simultaneously hacking away at the code. Remember, common engineering wisdom says that writing code is only 10% of the job. The rest is testing, maintaining the the codebase and communicating changes, and you will be graded in all of those actively. Practice this workflow until it gets under your skin.
