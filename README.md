# Still a work in progress

# What is this repository ?

This project aims to create a generic thread pool in c++, and will be a part of the [Arfang](https://github.com/Linsexy/arfang-engine) engine.

**This project requires a c++17 compliant compiler**


# What is a thread pool ?

In order to understand what is a thread pool, it's better (mandatory ?) to be comfortable with the notion of [threads](https://en.wikipedia.org/wiki/Thread_(computing))

Well, if you are, you should know that working with multithreaded programs is not easy, and so for plenty of reasons.

For example, have to manually assign threads to your functions, and you have to *instanciate* software threads, which is heavy for your computer. A threadpool solves this problem by instanciating threads at the beginning of your program and maintaining them in an *idle* state until you specify them to do some work.

# What are the other advantages ?

This project let you define *asynchronous* tasks. It frees you from declaring your threads, and eventually modifying your program architecture to be multithreaded by allowing you to access easily to wheter your tasks are finished and to their return types.

# TODO:
-example

-improvement of the api
