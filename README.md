# Sig
When writing other projects, I found that sometimes I would need to perfom cleanups or 'undo' certain actions when the program exited be it a normal or abnormal termination. This always seemed like a painful endeavour: lots of boilerplate, clunky interfaces, constantly storing values in global variables so the signal handler could use it.

I think it always turned out this way because I went for a fast-and-simple approach rather than taking the time to map out what I really wanted to do. So here I am, mapping out what I really want to do and rolling it all into a convenient library so that I, and others, can make use of it.
