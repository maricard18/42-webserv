OLAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
None
<!--: spam
Content-Type: text/html

<body bgcolor="#f0f0f8"><font color="#f0f0f8" size="-5"> -->
<body bgcolor="#f0f0f8"><font color="#f0f0f8" size="-5"> --> -->
</font> </font> </font> </script> </object> </blockquote> </pre>
</table> </table> </table> </table> </table> </font> </font> </font><body bgcolor="#f0f0f8">
<table width="100%" cellspacing=0 cellpadding=2 border=0 summary="heading">
<tr bgcolor="#6622aa">
<td valign=bottom>&nbsp;<br>
<font color="#ffffff" face="helvetica, arial">&nbsp;<br><big><big><strong>TypeError</strong></big></big></font></td
><td align=right valign=bottom
><font color="#ffffff" face="helvetica, arial">Python 3.8.10: /usr/bin/python3<br>Mon Nov  6 20:01:21 2023</font></td></tr></table>
    
<p>A problem occurred in a Python script.  Here is the sequence of
function calls leading up to the error, in the order they occurred.</p>
<table width="100%" cellspacing=0 cellpadding=0 border=0>
<tr><td bgcolor="#d8bbff"><big>&nbsp;</big><a href="file:///nfs/homes/maricard/Documents/cursus/webserver/miniserver/cgi-bin/cgi_post.py">/nfs/homes/maricard/Documents/cursus/webserver/miniserver/cgi-bin/cgi_post.py</a> in <strong>&lt;module&gt;</strong></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;20</small>&nbsp;<br>
</tt></font></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;21</small>&nbsp;#&nbsp;Check&nbsp;if&nbsp;the&nbsp;folder&nbsp;exists,&nbsp;if&nbsp;not&nbsp;create&nbsp;it<br>
</tt></font></td></tr>
<tr><td bgcolor="#ffccee"><tt>=&gt;<small>&nbsp;&nbsp;&nbsp;22</small>&nbsp;if&nbsp;not&nbsp;os.path.exists(uploads_folder):<br>
</tt></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;23</small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;os.makedirs(uploads_folder)<br>
</tt></font></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;24</small>&nbsp;<br>
</tt></font></td></tr>
<tr><td><small><font color="#909090"><strong>os</strong>&nbsp;= &lt;module 'os' from '/usr/lib/python3.8/os.py'&gt;, os.<strong>path</strong>&nbsp;= &lt;module 'posixpath' from '/usr/lib/python3.8/posixpath.py'&gt;, os.path.<strong>exists</strong>&nbsp;= &lt;function exists&gt;, <strong>uploads_folder</strong>&nbsp;= None</font></small></td></tr></table>
<table width="100%" cellspacing=0 cellpadding=0 border=0>
<tr><td bgcolor="#d8bbff"><big>&nbsp;</big><a href="file:///usr/lib/python3.8/genericpath.py">/usr/lib/python3.8/genericpath.py</a> in <strong>exists</strong>(path=None)</td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;17</small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"""Test&nbsp;whether&nbsp;a&nbsp;path&nbsp;exists.&nbsp;&nbsp;Returns&nbsp;False&nbsp;for&nbsp;broken&nbsp;symbolic&nbsp;links"""<br>
</tt></font></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;18</small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;try:<br>
</tt></font></td></tr>
<tr><td bgcolor="#ffccee"><tt>=&gt;<small>&nbsp;&nbsp;&nbsp;19</small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;os.stat(path)<br>
</tt></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;20</small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;except&nbsp;(OSError,&nbsp;ValueError):<br>
</tt></font></td></tr>
<tr><td><font color="#909090"><tt>&nbsp;&nbsp;<small>&nbsp;&nbsp;&nbsp;21</small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;False<br>
</tt></font></td></tr>
<tr><td><small><font color="#909090"><em>global</em> <strong>os</strong>&nbsp;= &lt;module 'os' from '/usr/lib/python3.8/os.py'&gt;, os.<strong>stat</strong>&nbsp;= &lt;built-in function stat&gt;, <strong>path</strong>&nbsp;= None</font></small></td></tr></table><p><strong>TypeError</strong>: stat: path should be string, bytes, os.PathLike or integer, not NoneType
<br><tt><small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</small>&nbsp;</tt>args&nbsp;=
('stat: path should be string, bytes, os.PathLike or integer, not NoneType',)
<br><tt><small>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</small>&nbsp;</tt>with_traceb

