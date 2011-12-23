
FirstName: {$FirstName}
{foreach from=$case1 item=x}
{$x.first}
{foreach from=$case2 item=y}
{$y.first}
{$y.second}
{foreachelse}
This is second Foreach else
{/foreach}
{$x.second}
{foreachelse}
This is Foreach else
{/foreach}
