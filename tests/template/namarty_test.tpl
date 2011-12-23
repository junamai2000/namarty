{foreach from=$case1 item=i}
--outside--{$i.first}
--outside--{$i.second}
{foreach from=$case2 item=j}
--inside--{$j.first}
--inside--{$j.second}
{/foreach}
{/foreach}
