pipy()
    .listen(8080)
        .decodeHTTPRequest()
        .replaceMessage(() => new Message('Hello!\n'))
        .encodeHTTPResponse()