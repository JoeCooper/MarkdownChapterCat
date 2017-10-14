# Markdown ChapterCat
Concatenate markdown files together as chapters, with a table of contents.

Given a set of markdown files representing different chapters or sections of a text, write a file which associates filenames of these texts with section names:

```
chapter1.md;Chapter One: The Great Escape
chapter2.md;Chapter Two: The Burlap Sack of Magic
chapter3.md;Chapter Three: Toxoplasmosis
```

Then, given a build of chaptercat, run it, passing the name of the chapter list file as the sole parameter:

```
./chaptercat chapters
```

ChapterCat will output the files together as a single document with a table of contents which hyperlinks to each section in the document.

ChapterCat writes to standard output, so it can be piped into markdown, or concatenated with a header and footer page.

ChapterCat is written in plain C and can be compiled with gcc with no special parameters:

```
gcc chaptercat.c -o chaptercat
```

Additionally, ```Makefile.am``` and ```configure.ac``` are added to provide compatability with autotools.
