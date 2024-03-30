let liquidHeight = 0;

//need to check if the order is right tho
function addLiquid() {
  if (liquidHeight < 200) { // Max height of liquid
    liquidHeight += 20; // Increase the height of the liquid by 20px
    document.getElementById('liquid').style.height = liquidHeight + 'px';
  }
}




