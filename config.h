/* configuration:
   include lines like this:
   { "output to be followed", "output that follows" },
   the second entry of the line will be connected to any sources
   that are connected to the first entry in the line, and will
   also follow disconnects.
*/

struct pair pairs[] = {
	{"system:playback_1", "q12out:playback_1"},
	{"system:playback_2", "q12out:playback_2"},
};
