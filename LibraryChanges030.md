# libifmap2c-0.3.0 #
10. July 2011

> The "Lets break things" release ;)

## Changes ##

  * Switch the arguments of SSRC::purge(), The publisher-id goes first, then the session-id. This makes it consistent with the other SSRC methods.
  * Rename ErrorResultError into ErrorResult.
  * Add a name attribute to ErrorResult, to be set if the ErrorResult is contained in a PollResult (name of subscription).
  * Create IfmapError as superclass for all errors except ErrorResult. A user therefore only needs to catch IfmapError and ErrorResult.

  * IfmapError and ErrorResult now support output to a stream. No need for the `getMessage()` calls anymore, e.g:
```
    try {
       ssrc.newSession();
       ssrc.publish(something);
       ssrc.endSession();
    } catch (IfmapError ifmapError) {
       cerr << ifmapError << endl;
    } catch (ErrorResult errorResult) {
       cerr << errorResult << endl;
    }
```
  * Rename SubPublish and SubSubscribe to PublishElement and SubscribeElement, respectively. The old names were just horrible.
  * SearchResult::getResultItem(Identifier i1, Identifier i2), to  a single ResultItem element changed to return a list of ResultItem objects, as there might be multiple ResultItems with the same Identifiers (depending on the MAPS used).
  * Disable "pretty printing" to generate the resulting XML document.
  * Rename SearchResult::getSearchResultName() into ::getName().
  * SSRC::newSession() max-poll-result-size parameter changed to int.
  * Add two more createPublishUpdate() methods which make creation of updates containing a link "look better".
  * Introduce a UserMetadata class, which can be used to generate own metadata types.
  * Rename ResultItem::getMetadataList() to ::getMetadata()

## Bugfixes ##

  * Rename InvalidSessionId back to InvalidSessionID...
  * PollResult objects now correctly may contain ErrorResult objects.
  * Revisited parsing of PollResults. It was "pretty broken" before.
  * Uninitialized iterator dereferencing in PollResult fixed.
  * Don't expect an errorString to be set in all ErrorResults,