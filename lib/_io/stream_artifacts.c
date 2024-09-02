

//	INVALID test cases
void new_stream_c()
{
	enum io_mode mode = CREATE;
	writefln("Stream.new  %s", get_mode_label(mode));

	file *pFile = File.new(pfTemp);
	stream *pStream;
	assert(Stream.new(pFile, mode, &pStream));
	assert(strcmp("\0", Stream.mode_info(pStream->mode)) == 0);

	__output_stream_info(pStream);
	if (File.exists(pFile))
	{
		writefln("deleting %s", pFile->name);
		File.delete(pFile);
	}

	Stream.free(pStream);
}
void new_stream_r_fail()
{
	enum io_mode mode = READ;
	writefln("Stream.new(ERROR: %s)  %s", Stream.err_info(FILE_NOT_FOUND), get_mode_label(mode));

	stream *pStream;
	//	TODO: Stream.get_error(pStream)
	assert(!Stream.new(File.new(pfTemp), mode, &pStream));
	assert(strcmp("r", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void new_stream_r_pass()
{
	enum io_mode mode = READ;
	writefln("Stream.new(  %s", get_mode_label(mode));

	file *pFile = File.new(pfTemp);
	File.create(pFile);
	stream *pStream;

	assert(Stream.new(pFile, mode, &pStream));
	assert(strcmp("r", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	if (File.exists(pStream->source))
	{
		writefln("deleting %s", pStream->source->name);
		File.delete(pStream->source);
	}
	Stream.free(pStream);
}
void new_stream_rw_fail()
{
	enum io_mode mode = READ | WRITE;
	writefln("Stream.new(ERROR: %s)  %s", Stream.err_info(FILE_NOT_FOUND), get_mode_label(mode));

	stream *pStream;

	assert(!Stream.new(File.new(pfTemp), mode, &pStream));
	assert(strcmp("r+", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void new_stream_rw_pass()
{
	enum io_mode mode = READ | WRITE;
	writefln("Stream.new  %s", get_mode_label(mode));

	file *pFile = File.new(pfTemp);
	File.create(pFile);
	stream *pStream;

	assert(Stream.new(pFile, mode, &pStream));
	assert(strcmp("r+", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	if (File.exists(pStream->source))
	{
		writefln("deleting %s", pStream->source->name);
		File.delete(pStream->source);
	}
	Stream.free(pStream);
}
void new_stream_rwc()
{
	enum io_mode mode = READ | WRITE | CREATE;
	writefln("Stream.new  %s", get_mode_label(mode));
	stream *pStream;

	assert(Stream.new(File.new(pfTemp), mode, &pStream));
	assert(strcmp("r+", Stream.mode_info(pStream->mode)) == 0);
	__output_stream_info(pStream);
	if (File.exists(pStream->source))
	{
		writefln("deleting %s", pStream->source->name);
		File.delete(pStream->source);
	}
	Stream.free(pStream);
}
void read_stream_c_fail()
{
	/*
		NOTE: pfSource (./.data/main.C) already exists so CREATE actually does nothing.

		When a stream is created (.new(...)) with only CREATE mode, a file stream has not yet
		been supplied by the system. We have a couple of options demonstrated in the next test cases
	*/
	enum io_mode mode = CREATE;
	writefln("Stream.read(ERROR: %s)  %s", Stream.err_info(BAD_MODE), get_mode_label(mode));
	stream *pStream;

	Stream.new(File.new(pfSource), mode, &pStream);
	char c;
	assert(!Stream.read(pStream, &c));
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void read_stream_open()
{
	enum io_mode mode = CREATE;
	enum io_mode expMode = READ;
	writefln("Stream.open  %s --> %s", get_mode_label(mode), get_mode_label(expMode));
	stream *pStream;

	/*
		Option 1: when a stream object returns without error, call `.open(...)` with the desired
		modes - READ, READ | WRITE, APPEND, etc - to have the modes set and retrieve a file stream
		handle from the system.
	*/
	Stream.new(File.new(pfSource), mode, &pStream);
	assert(Stream.open(pStream, expMode));
	assert(pStream->mode == expMode);

	char c;
	assert(Stream.read(pStream, &c));
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void read_stream_c_pass()
{
	enum io_mode mode = CREATE;
	enum io_mode expMode = READ;
	writefln("Stream.open  %s --> %s", get_mode_label(mode), get_mode_label(expMode));
	stream *pStream;

	/*
		Option 2: when a stream object returns without error, set the `mode` to something compatible
		with the operation - READ, READ | WRITE, APPEND, etc. This will prevent Stream from returning
		false with an *incompatible mode* complaint.
	*/
	Stream.new(File.new(pfSource), mode, &pStream);
	pStream->mode = expMode;

	char c;
	assert(Stream.read(pStream, &c));
	__output_stream_info(pStream);
	Stream.free(pStream);
}
void close_stream()
{
	enum io_mode mode = READ;
	writefln("Stream.close  %s", get_mode_label(mode));
	stream *pStream;

	/*
		Option 2: when a stream object returns without error, set the `mode` to something compatible
		with the operation - READ, READ | WRITE, APPEND, etc. This will prevent Stream from returning
		false with an *incompatible mode* complaint.
	*/
	Stream.new(File.new(pfSource), mode, &pStream);
	assert((pStream->status & OK) == OK);

	Stream.close(pStream);
	assert((pStream->status & CLOSED) == CLOSED);
}
