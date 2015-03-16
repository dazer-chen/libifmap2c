# libifmap2c-examples 0.3.0 (10. July 2011) #
## Changes ##
  * Adapted code to compile with 0.3.0, simplify exception handling, split things a bit.
  * Include a folder `utils/` which besides `ip-mac`, also provides other utilities to publish some standard metadata. Nice for testing and scripting. These tools optionally use environment variables for url. user, password and capath. Those are called `IFMAP_URL`, `IFMAP_USER`, `IFMAP_PASSWORD` and `IFMAP_CAPATH`, makes life easier.
  * Add a folder `tests`, with some code to mainly test the library, but some MAPS functionality at the same point. Note, I mainly checked against irond (0.2.3). omapd or IBOS might lead to different results. If you think the expected behavior is  let me know.

As a note: The tests use identifier values which are based on the basic-authentication username. This is done in order to allow concurrent executions with different users without interfering each other.