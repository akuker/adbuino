Source code for http://tvc-16.science.
It's all svg look how lightweight it is holy fuck

I use two shitty crontabs for writing the uptime files:

* * * * * uptime | awk -F'( |,|:)+' '{print $6,$7}' > /var/www/html/uptime
* * * * * uptime | awk -F'( |,|:)+' '{print $6,$7",",$8,"hours,",$9,"minutes."}' > /var/www/html/uptimedetail

As a bonus, tvc-16.yml contains the docker-compose configuration used to deploy the main website.