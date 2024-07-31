import Protocols.HTTP;

int main()
{
	write("Welcome to the OpenPlatform Network Browser\n");
	string url;
	write("Enter web address: ");
	url = Stdio.stdin->gets();
	url = validate_url(url);

	write("\nFetching URL: " + url + "\n");

	Query web_page = get_url(url);
	if (web_page == 0)
	{
		write(" Failed.\n");
		return -1;
	}

	output_url(web_page);

	return 0;
}

string validate_url(string url) {
	string prefix = "https://";
	if(search(url, prefix) != 1) {
		url = prefix + url;
	}
	return url;
}

void output_url(Query web_page) {
	string page_content = web_page->data();
	write(page_content + "\n");
}