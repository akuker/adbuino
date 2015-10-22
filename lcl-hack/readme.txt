Script for getting your money stats from Crédit Lyonnais. 
Requires PhantomJS.

Syntax:

phantomjs lcl.js <identifiant> <code> (<agence> <numcompte> <lettrecle>)
Only giving ID and code will list all accounts for your bank account.
Specifying an account will give its latest expenses and gains.

Outputs on STDOUT, in an RSS file, in a Windows 8/10-compatible Live Tile(usable by pinning the given index.html in IE/Edge), and in a plain text file.