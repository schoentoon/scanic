scanic
======

IRC log analyzer. Designed to be very modular.
The irc logs can be in any format, as long as there's an input module that parses it correctly.

As for the analyzing this is fully modular as well. You can easily write your own analyzers.

As for the actual output, this is done through [SMART-TPL](https://github.com/CopernicaMarketingSoftware/SMART-TPL).
You'll have to write your own template for your output, in here you can display any of your analyzers of choice.
If you don't like one, simply don't show it.
