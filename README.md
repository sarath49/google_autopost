Google+ Autopost to Streams using Domains API.
----------------------------------------------

Contents of File
----------------
* Introduction
* Requirements
* Setup & Installation
* Configuration
* Notes
* Maintainers

Introduction
------------

Google+ Autopost allows posting of content to Google+ Streams using 
Google+ Domains API.

Requirements
------------

1. Google G Suit Account.
Note: Your normal Gmail acccount won't suffix for this and you will need to have
a Google G Suit Account Registered using a domain name.
Link: https://gsuite.google.com

2. Google APIs Client Library for PHP.
Note: This module uses 1.0.5-beta version of Google APIs Client Library for PHP.
If you want to use latest version modify code as per the version you desire.
Link: https://github.com/google/google-api-php-client/tree/1.0.5-beta

3. Drupal 7

Setup & Installation
--------------------

1. Create a Google G Suit Account using your registered public domain.

2. Follow the instructions provided for
 a) Create a console project with service account credentials
 b) Delegate domain-wide authority to your service account
 from links below:
 a) https://developers.google.com/+/domains/authentication/delegation

3. Download the Google APIs Client Library for PHP version 1.0.5-beta, extract 
and place it inside libraries folder. Rename the folder to google-api-php-client
& make sure autoload.php is at the root folder of google-api-php-client folder.

4. Install Google+ Autopost module.

Configuration
-------------

1. Go to admin/config/services/google-autopost. 
Fill in all required information and save configuration.

2. Perform Cache Clear /admin/config/development/performance

3. Go to admin/config/services/google-autopost/auth and 
click Authenticate Google+ button.
This will redirect to the Google Authentication Page. Click on Allow and 
will redirect back to your website.

(Note: If you are not redirected, check if overlay is disabled. You need to 
disable overlay module or remove the overlay added text in link to get clear url
then click on Authenticate Google+.)

4. Now you are ready to Autopost to your Google+ Stream. 


Notes
-----

1. This module does not provide all functionality as what is provided by 
Google+ Domains API and has limited scope of just writing into streams.

2. All post thats are being posted to Google+ Streams are not visible public and
only visible to users belonging to same domain. If you want some post to be
visible publicly then you will have to perform it manually.

Maintainers
-----------

Sarath Kumar M https://www.drupal.org/u/sarathkm


References
----------

1. https://developers.google.com/+/domains/
2. https://developers.google.com/+/domains/posts/creating
3. https://support.google.com/plus/answer/2705802

