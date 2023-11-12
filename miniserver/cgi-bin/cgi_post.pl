# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgi_post.pl                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:25 by maricard          #+#    #+#              #
#    Updated: 2023/11/12 12:54:58 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $cgi = CGI->new;

my $uploadsFolder = $ENV{'UPLOAD_STORE'};
my $location;

if (!$uploadsFolder || $uploadsFolder ne 'uploads') {
    $location = '(check configuration file)';
} else {
    $location = $uploadsFolder;
}

# Check if the folder exists, if not create it
if (!-e $uploadsFolder && !mkdir($uploadsFolder, 0755)) {
    die "Failed to create upload directory";
}

my $message;

my $filename = $cgi->param('filename');

if (!$filename) {
    $message = 'No file was uploaded';
} else {
    my $uploadPath = "$uploadsFolder/$filename";

    if (!-e $uploadPath) {
        my $filehandle = $cgi->upload('filename');

        open my $outfile, '>', $uploadPath or die "Failed to open file: $!";
        binmode $outfile;

        while (my $bytesread = read $filehandle, my $buffer, 1024) {
            print $outfile $buffer;
        }

        close $outfile;

        $message = "The file \"$filename\" was uploaded successfully to /$location";
    } else {
        $message = "The file \"$filename\" already exists";
    }
}

print $cgi->header('text/plain');
print $message;