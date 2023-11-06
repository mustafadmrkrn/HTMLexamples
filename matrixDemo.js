const kodfun = document.getElementById("kodfun");
let lines = [
  "Uyan Artik Sezin",
  "The Matrix tam olarak sensin...",
  "Yesil Tavsani takip et.",
  "Sana yapman gerekenleri soyleyecek."
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
