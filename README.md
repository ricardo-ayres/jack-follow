# jack-follow

jack-follow is small C program using the
[JACK](https://github.com/jackaudio/jack2) API.  The idea is to
automatically copy any connections made to one output to another.  I made
this because I have two USB soundcards, I run jackd with one of them and
[alsa_out](https://github.com/jackaudio/tools) with the other, and I
wanted to make any autoconnections of system outputs to be 'followed'
by the card exposed to jack with alsa_out.

The configuration is done in config.h, by specifying the leading output
and the following or 'mirror' output:

```C struct pair pairs[] = {
	{"system:playback_1", "q12out:playback_1"},
	{"system:playback_2", "q12out:playback_2"},
};
```

This means that, for instance, if mpv starts and connects to system
outputs system:playback_{1,2}, jack-follow will connect q12out:playback_{1,2}
to whatever sources were connected to the system outs, in this case, mpv.

# Disclaimer

This is provided as is. I've written this for my own personal use.
