--[[
This lua function is used by fluent bit to strip the containerd log prefix from the middle of multiline logs
 (which looks like \n2020-08-17T21:14:00.453086985Z stdout F )

 ex: strip_header(_, _, "20200817211400452 [main] WARN  testlogger11 - this is a single line warn\n2020-08-17T21:14:00.452960883Z stdout F 20200817211400452 [main] FATAL testlogger11 - generated an exception")
 returns: (1, _, "20200817211400452 [main] WARN  testlogger11 - this is a single line warn 20200817211400452 [main] FATAL testlogger11 - generated an exception")
]]

function strip_header(tag, timestamp, record)
    cleaned, n_replaced = record["log"]:gsub("\n%d+-%d+%d+-%d+T%d+:%d+:%d+.%d+Z std[oe][ue][tr] [FP]","")
    if n_replaced == 0 then
      return 0, timestamp, record  -- return code 0 signals no change
    else
      record["log"] = cleaned
      return 1, timestamp, record  -- return code 1 signals timestamp and record could have changed
    end
  end
