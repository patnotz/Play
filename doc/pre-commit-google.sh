#!/bin/sh
#
# An example hook script to verify what is about to be committed.
# Called by "git commit" with no arguments.  The hook should
# exit with non-zero status after issuing an appropriate message if
# it wants to stop the commit.
#
# To enable this hook, rename this file to "pre-commit".

if git rev-parse --verify HEAD >/dev/null 2>&1
then
	against=HEAD
else
	# Initial commit: diff against an empty tree object
	against=4b825dc642cb6eb9a060e54bf8d69288fbee4904
fi

# If you want to allow non-ascii filenames set this variable to true.
allownonascii=$(git config hooks.allownonascii)

# Cross platform projects tend to avoid non-ascii filenames; prevent
# them from being added to the repository. We exploit the fact that the
# printable range starts at the space character and ends with tilde.
if [ "$allownonascii" != "true" ] &&
	# Note that the use of brackets around a tr range is ok here, (it's
	# even required, for portability to Solaris 10's /usr/bin/tr), since
	# the square bracket bytes happen to fall in the designated range.
	test "$(git diff --cached --name-only --diff-filter=A -z $against |
	  LC_ALL=C tr -d '[ -~]\0')"
then
	echo "Error: Attempt to add a non-ascii file name."
	echo
	echo "This can cause problems if you want to work"
	echo "with people on other platforms."
	echo
	echo "To be portable it is advisable to rename the file ..."
	echo
	echo "If you know what you are doing you can disable this"
	echo "check using:"
	echo
	echo "  git config hooks.allownonascii true"
	echo
	exit 1
fi

show_uncrustify_diff=true
show_cpplint_output=true
proj_root_rel=$(git rev-parse --show-cdup)
uncrustify_cfg=${proj_root_rel}doc/google.cfg
cpplint_py="${proj_root_rel}doc/cpplint.py --filter=-build/header_guard,-runtime/int"

skip_cpp_format_check() {
	 ext=${1##*.}
	 cpp_ext="C cpp cxx H h hpp hxx"
	 for e in $cpp_ext ; do
		  if test "$ext" = "$e" ; then
				return 1
		  fi
	 done
	 return 0
}

check_format() {
	 working_file=$1
	 actual_file=$2
    tmpfile=$working_file.uncrustified
	 uncrustify_cmd="uncrustify -c $uncrustify_cfg -l CPP"
	 $uncrustify_cmd -f $working_file -o $tmpfile 2>/dev/null
	 diff -q $working_file $tmpfile >/dev/null 2>&1 
	 crusty=$?
	 if test 0 -eq $crusty ; then
		  echo "    uncrustify check: clean."
	 else
		  echo "    uncrustify check: dirty - please uncrustify:"
		  echo "        $uncrustify_cmd $actual_file"
		  if $show_uncrustify_diff ; then
				diff $working_file $tmpfile
		  fi
    fi
	 rm -f $tmpfile
	 return $crusty
}

check_lint() {
	 working_file=$1
	 actual_file=$2
	 $cpplint_py $working_file >/dev/null 2>&1
	 linty=$?
	 if test 0 -eq $linty ; then
		  echo "    cpplint.py check: clean."
	 else
		  echo "    cpplint.py check: dirty - please delint:"
		  echo "        $cpplint_py $actual_file"
		  if $show_cpplint_output ; then
				$cpplint_py $working_file
		  fi
    fi
	 return $linty
}

check_file() {
	 file=$1
	 echo "pre-commit-google.sh: checking $file..."

	 # Be careful of partial commits -- work on the files in the git index
	 ext=${1##*.} # preserve the extension
	 actual_file=$proj_root_rel$file
	 working_file=${actual_file%.*}~pre-commit-google.$ext
	 git show :$actual_file > $working_file

	 file_err=0

	 check_format $working_file $actual_file
    if test 0 -ne $? ; then
		  file_err=1
	 fi
	 check_lint $working_file $actual_file
    if test 0 -ne $? ; then
		  file_err=1
	 fi
	 
	 if test 1 -eq $file_err ; then
	     # If the user is committing only parts of the working copy
	     # warn them because the uncrustify or cpplint output may
        # be inconsistent with their working copy
		  diff -q $working_file $actual_file >/dev/null 2>&1 
		  partial_commit=$?
		  if test 0 -ne $partial_commit ; then
				echo
				echo "NOTICE: you're only committing some of the the changes in $actual_file"
				echo "The output from uncrustify or cpplint.py may be different from"
				echo "that of your working copy"
				echo
		  fi
	 fi
	 rm -f $working_file
	 return $file_err
}

cmt_err=0
for file in $(git diff --cached --name-only); do

	 if skip_cpp_format_check $file ; then 
		  echo "pre-commit-google.sh: skipping non-C++ file $file"
	 else
		  check_file $file
		  file_err=$?
		  if test 0 -ne $file_err ; then
				cmt_err=1
		  fi
	 fi
done
exit $cmt_err
