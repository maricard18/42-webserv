# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    cgi_post.pl                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maricard <maricard@student.porto.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/18 17:24:25 by maricard          #+#    #+#              #
#    Updated: 2023/11/14 12:22:51 by maricard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use Data::Dumper; warn Dumper [<STDIN>];
use File::Path qw(make_path);

my $cgi = CGI->new;

my $uploadsFolder = $ENV{'UPLOAD_STORE'};
my $location;

if (!$uploadsFolder || $uploadsFolder ne 'uploads')
{
    $location = '(check configuration file)';
}

else
{
    $location = $uploadsFolder;
}

unless (-d $uploadsFolder)
{
    make_path($uploadsFolder, { mode => 0755 }) or die "Failed to create upload directory: $!";
}

my $message;
my $filename = $cgi->param('filename');

if (!$filename)
{
    $message = 'No file was uploaded';	
}
else
{
    my $uploadPath = "$uploadsFolder/$filename";

    if (!-e $uploadPath)
	{
        my $filehandle = $cgi->upload('filename');

        open my $outfile, '>', $uploadPath or die "Failed to open file: $!";
        binmode $outfile;

        while (my $bytesread = read $filehandle, my $buffer, 1024)
		{
            print $outfile $buffer;
		}

        close $outfile;

        $message = "The file \"$filename\" was uploaded successfully to /$location";
    }
	else
	{
        $message = "The file \"$filename\" already exists";
	}
}

print $cgi->header('text/plain');
print $message;