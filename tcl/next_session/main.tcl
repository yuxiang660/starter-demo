proc find_max_session_number {} {
  set sessions [glob -nocomplain *.session]
  set max_session_number 0
  foreach session $sessions {
    set dash_position [expr [string first _ $session]+1]
    set dot_position [expr [string first . $session]+1]
    set session_number [string range $session $dash_position $dot_position-2]
    if {$session_number > $max_session_number} {
      set max_session_number $session_number
    }
  }
  return $max_session_number
}

proc get_next_session {} {
  set sessions [glob -nocomplain *.session]
  set max_session_number 0
  foreach session $sessions {
    set dash_position [expr [string first _ $session]+1]
    set dot_position [expr [string first . $session]+1]
    set session_number [string range $session $dash_position $dot_position-2]
    if {$session_number > $max_session_number} {
      set max_session_number $session_number
    }
  }
  return "run_[expr $max_session_number+1]"
}

set SESSION_NAME [get_next_session]
puts "STRESS: Using session name '$SESSION_NAME' in xeDebug"
# set MAX_SESSION_NUMBER [find_max_session_number]
# puts "STRESS: Using session number '$MAX_SESSION_NUMBER' in xeDebug"