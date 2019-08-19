<?php

/**
 * @file
 * islandora-matomo-widget.tpl.php
 */
?>
<?php if ($matomo_token): ?>
<div class="islandora-matomo-count">
    <div class="stats-box">
        <div class="row">
            <div class="col-sm-6">
                <span class="stats-data"><?php print $stats['visits']; ?></span>
            </div>
            <div class="col-sm-6">
                &nbsp;visits<i class="fa fa-eye " data-toggle="tooltip" tooltip="" data-placement="top" title="" data-original-title="Visits."></i>
            </div>
        </div>
        <div class="row">
            <div class="col-sm-6">
                <span class="stats-data"><?php print $stats['downloads']; ?></span>
            </div>
            <div class="col-sm-6">
                &nbsp;dowloads<i class="fa fa-download " data-toggle="tooltip" tooltip="" data-placement="top" title="" data-original-title="Downloads."></i>
            </div>
        </div>
    </div>
</div>
<?php endif;?>
