const kodfun = document.getElementById("kodfun");
let lines = [
  "Wake up, Neo...",
  "The Matrix has you...",
  "Follow KodFun on Instagram.",
  "Knock, knock, Neo."
];
let i = 0,
  j = 0,
  wait = 6;
setInterval(() => {
  if (wait-- > 0) return;
  if (wait == -1) kodfun.innerHTML = "";
  if (i < lines.length)
    if (j < lines[i].length) kodfun.innerHTML += lines[i][j++];
    else (wait = 8), (j = 0), i++;
  else (wait = 8), (i = j = 0);
}, 200);
