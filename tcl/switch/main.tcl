set ::env(EMULATOR) hsv-smd415.cadence.com

if {[info exists ::env(EMULATOR)]} {
  switch $::env(EMULATOR) {
    hsv-smd415.cadence.com -
    hsv-smd415 {
        puts "Emulator hsv-smd415"
    }
    hsv-smd416 {
        puts "Emulator hsv-smd416"
    }
    default {
        puts "!!! Error: Emulator '$::env(EMULATOR)' does not have hdsb configuration. !!!"
        exit 1
    }
  }
  puts "Your host is $::env(EMULATOR)"
}
