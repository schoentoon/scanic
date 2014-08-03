{foreach $linecount as $nick => $lines}
  {$nick}: {$lines}
{foreachelse}
  Nobody said anything? How boring.
{/foreach}
linecount.so took {$timing.linecount}s