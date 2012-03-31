import sys, os.path

# Not really part of the exercise and that's why I'm using Python.
# Copying the message to decipher from the PDF sadly results in
# mixed up words. The rows and columns have to be switched first.

# NOTE: Only works if rows and columns are full, no open cells.


if __name__ == "__main__":
	if len( sys.argv ) < 5:
		print "Expected parameters:"
		print "    Input file\t\t\t- file_to_read.txt"
		print "    Output file\t\t\t- file_to_write.txt"
		print "    Rows in the aimed for output\t- 6"
		print "    Columns in the aimed for output\t- 7"
		sys.exit( 2 )

	from_file = sys.argv[1]
	to_file = sys.argv[2]
	rows = int( sys.argv[3] )
	cols = int( sys.argv[4] ) - 1

	if os.path.isfile( to_file ):
		print "File " + to_file + " already exists. I don't want to overwrite your stuff."
		print "Please choose a name without an existing file to it."
		sys.exit( 2 )

	# get input file
	file_in = open( from_file, "r" );

	# clean up line breaks
	msg = []
	for word in file_in.readlines():
		word = word.replace( "\n", "" )
		if( word != "" ):
			msg.append( word )

	file_in.close()

	# switch rows and columns
	lines = []
	for col in range( 0, cols ):
		line = msg[col::rows]
		if len( line ) == cols + 1:
			lines.append( line )

	# write result to file
	file_out = open( to_file, "w" )
	for line in lines:
		for word in line:
			file_out.write( word )
		file_out.write( "\n" )
	file_out.close()

	print "Done."