<html>
  <body>
    <table>
      {foreach $linecount as $nick => $lines}
        <tr><td>{$nick}</td><td>{$lines}</td></tr>
      {foreachelse}
        <tr><td colspan=2>Nobody said anything? How boring.</td></tr>
      {/foreach}
      <tr><td colspan=2>linecount.so took {$timing.linecount}s</td></tr>
    </table>
    <table>
      {foreach $smileycount as $smiley => $count}
        <tr><td>{$smiley}</td><td>{$count}</td></tr>
      {foreachelse}
        <tr><td colspan=2>No smilies? How boring :(</td></tr>
      {/foreach}
      <tr><td colspan=2>smileycount.so took {$timing.smileycount}s</td></tr>
    </table>
    <table>
      {foreach $wordcount as $nick => $count}
        <tr><td>{$nick}</td><td>{$count}</td></tr>
      {foreachelse}
        <tr><td colspan=2>Nothing was said? BORING.</td></tr>
      {/foreach}
      <tr><td colspan=2>wordcount.so took {$timing.wordcount}s</td></tr>
    </table>
    <table>
      {foreach $caps as $nick => $count}
        <tr><td>{$nick}</td><td>{$count}</td></tr>
      {foreachelse}
        <tr><td colspan=2>No yelling here, thankfully</td></tr>
      {/foreach}
      <tr><td colspan=2>caps.so took {$timing.caps}s</td></tr>
    </table>
  </body>
</html>
