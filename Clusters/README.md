<h1>Clusters</h1>

<p>
I have written an algorithm which can order and sort large numbers of items.<br />
It creates a directory-tree in form of a pyramid, moving the entries and groups accordingly.<br />
You can find further information on my website at
<a href="http://svenbieg.azurewebsites.net/Clusters" target="_blank">http://svenbieg.azurewebsites.net/Clusters</a>.
</p><br />

<a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/Pyramid.jpg" style="" /></a><br />
<br />

<p>
The algorithm is provided in form of C++-templates, free for non-commercial use.<br />
I cannot give any warranty and i don't assume any liability for eventually caused damages.
</p><br />

<h2>Principle</h2><br />

<table>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/Group.png" /></a></td>
		<td>The entries are stored in groups.</td>
	</tr><tr><td></td></tr><tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/GroupFull.png" /></a></td>
		<td>The size of the groups is limited and 100 by default.</td>
	</tr><tr><td></td></tr><tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/ParentGroup.png" /></a></td>
		<td>If the group is full, a parentgroup is created.</td>
	</tr><tr><td></td></tr><tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;<a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/ParentGroup1.png" /></a></td>
		<td>The first and the last entry can be moved to the neighbour-group.</td>
	</tr><tr><td></td></tr><tr>
		<td><a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/ParentGroup2.png" /></a></td>
		<td>The entries are moved between the groups, so all groups get as full as possible.</td>
	</tr><tr><td></td></tr><tr>
		<td><a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/Level2Group.png" /></a></td>
		<td>The number of groups is limited too, another parentgroup is created.</td>
	</tr><tr><td></td></tr><tr>
		<td><a href="http://svenbieg.azurewebsites.net/Clusters"><img src="http://svenbieg.azurewebsites.net/Clusters/Level2Group1.png" /></a></td>
		<td>If an entry needs to be inserted in a full group, a whole subtree can be moved.</td>
	</tr>
</table><br />

<h2>List</h2>
<p>
The most simple cluster is the list. Items can be inserted and removed at random positions.<br /><br />
<a href="http://svenbieg.azurewebsites.net/Clusters/List"><img alt="List" src="http://svenbieg.azurewebsites.net/Clusters/List/List.jpg" /></a><br /><br />
It is more efficient than the list in the standard-library. You can find benchmarks on my website at
<a href="http://svenbieg.azurewebsites.net/Clusters/List" target="_blank">http://svenbieg.azurewebsites.net/Clusters/List</a>.
</p><br />

<h2>Index</h2>
<p>
The next cluster is the index, which is used for sorting. Each id can be linked with a user-defined entry.<br /><br />
<a href="http://svenbieg.azurewebsites.net/Clusters/Index"><img alt="Index" src="http://svenbieg.azurewebsites.net/Clusters/Index/Index.jpg" /></a><br /><br />
It is more efficient than the map-class in standard-library. You can find some benchmarks on my website at
<a href="http://svenbieg.azurewebsites.net/Clusters/Index" target="_blank">http://svenbieg.azurewebsites.net/Clusters/Index</a>.
</p><br />

<p>I hope my solution will make it into the standard-library!</p>

<p>Best regards,</p>

<p>Sven Bieg</p>

<br /><br /><br /><br /><br />
