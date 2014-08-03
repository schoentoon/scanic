<html>
  <body>
    <table>
      {foreach $linecount as $nick => $lines}
        <tr><td>{$nick}</td><td>{$lines}</td></tr>
      {foreachelse}
        <tr><td>Nobody said anything? How boring.</td></tr>
      {/foreach}
      <tr><td colspan=2>linecount.so took {$timing.linecount}s</td></tr>
    </table>
  </body>
</html>